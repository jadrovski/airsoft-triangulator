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

bool interactedWithUser = false;

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
    return false;
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
        interactedWithUser = true;
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

void setup()
{
    //Serial.begin(9600);

    pinMode(CONST::PIN_KEY_WAKE_UP, INPUT);

    pinMode(CONST::PIN_LED_1, OUTPUT);
    pinMode(CONST::PIN_LED_2, OUTPUT);
    pinMode(CONST::PIN_LED_4, OUTPUT);
    pinMode(CONST::PIN_LED_8, OUTPUT);

    pinMode(CONST::PIN_BTN_1, INPUT);
    pinMode(CONST::PIN_BTN_2, INPUT);

    //wipe mem
    MEM::writeLong(0, 0);
    MEM::writeLong(3, 0);
    MEM::writeLong(6, 0);

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
                interactedWithUser = readInteracted();
                if (!interactedWithUser)
                {
                    interactWithUserAction();
                }
                else if (interactedWithUser)
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
