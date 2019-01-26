#include <Arduino.h>
#include <LowPower.h>
#include "mem.h"
#include "const.h"
#include "control.h"
#include "led_display.h"
#include "buzzer.h"
#include "menu.h"
#include "game.h"

unsigned int tweetTimerMillis    = 0;
unsigned int interactTimerMillis = 0;

void welcome()
{
    for (int i = 0; i < 10; i++)
    {
        EXIT_FUNCTION_IF_NO_KEY();

        LED::displayNumberBinary(15);
        BUZZER::tweet(20);
        LED::clear();
        delay(20);
    }
}

void waitCommandBlink()
{
    LED::displayNumberBinary(15);
    delay(10);
    LED::clear();
    delay(1000);
}

bool readInteracted()
{
    return MEM::readULong(GAME::getConnectedKeyIdx() * 4) != 0;
}

void writeInteracted()
{
    MEM::writeULong(GAME::getConnectedKeyIdx() * 4, millis());
}

void wipeResults()
{
    for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++)
    {
        MEM::writeULong(i * 4, 0);
    }
}

void serviceAction()
{
    welcome();
    /*MENU::setMenuItem(2, 6);
    while (1)
    {
        MENU::displayInvalidate();
    }*/
}

void interactWithUserAction()
{
    while(1)
    {
        waitCommandBlink(); 
        CONTROL::updateState();

        interactTimerMillis = 0;

        while (
            CONTROL::areAllPressed() &&
            interactTimerMillis <= CONST::TIMELINE_INTERACT_MILLIS
        ) {
            unsigned long lastMillis = millis();
            delay(10);
            LED::displayNumberBinary(random(0, 15));
            interactTimerMillis += (millis() - lastMillis);
            
            CONTROL::updateState();

            EXIT_FUNCTION_IF_NO_KEY();
        }
        LED::clear();
        if (interactTimerMillis > CONST::TIMELINE_INTERACT_MILLIS)
        {
            writeInteracted();
            return;
        }
        
        EXIT_FUNCTION_IF_NO_KEY();
    }
}

void showRadiusAction()
{    
    int radius = GAME::getRadius();
    
    while (1) {
        delay(CONST::TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS);
        LED::displayNumberBlinking(radius);
        EXIT_FUNCTION_IF_NO_KEY();
    }
}

void printResultTable()
{
    for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++)
    {
        Serial.print("TEAM #");
        Serial.print(i);
        Serial.print(" CHECK-IN: ");
        long checkIn = MEM::readULong(i * 4);
        if (checkIn == 0)
        {
            Serial.print("NONE");
        }
        else
        {
            Serial.print(checkIn);
        }
        Serial.println();
    }
}

void setup()
{
    CONTROL::updateState();
    delay(CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS);
    delay(10);
    CONTROL::updateState();
    if (CONTROL::isBtn1Pressed() && !CONTROL::isBtn2Pressed())
    {
        wipeResults();
        BUZZER::tweet(1000);
    }
    
    Serial.begin(9600);

    printResultTable();

    pinMode(CONST::PIN_KEY_WAKE_UP, INPUT);

    pinMode(CONST::PIN_LED_1, OUTPUT);
    pinMode(CONST::PIN_LED_2, OUTPUT);
    pinMode(CONST::PIN_LED_4, OUTPUT);
    pinMode(CONST::PIN_LED_8, OUTPUT);

    pinMode(CONST::PIN_BTN_1, INPUT);
    pinMode(CONST::PIN_BTN_2, INPUT);

    attachInterrupt(0, GAME::updateKeyConnectedFlag, CHANGE);
}

void loop()
{
    GAME::updateKeyConnectedFlag();

    if (GAME::isKeyConnected())
    {
        delay(CONST::TIMELINE_BEFORE_WELCOME_DELAY_MILLIS);
        welcome();

        while (GAME::isKeyConnected())
        {
            if (GAME::isSuper())
            {
                serviceAction();
            } 
            else
            {
                bool interacted = readInteracted();
                if (!interacted)
                {
                    interactWithUserAction();
                }
                else
                {
                    showRadiusAction();
                }
            }
        }
    }
    else if (tweetTimerMillis >= CONST::TIMELINE_TWEET_DELAY_MILLIS)
    {
        BUZZER::tweet(CONST::TIMELINE_TWEET_IN_REST_DURATION_MILLIS);
        tweetTimerMillis = 0;
    }

    // going to sleep
    LED::clear();
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

    // awaken
    tweetTimerMillis += 1000;
}
