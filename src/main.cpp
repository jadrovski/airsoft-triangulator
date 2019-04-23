#include <Arduino.h>
#include <LowPower.h>
#include "mem.h"
#include "const.h"
#include "control.h"
#include "led_display.h"
#include "buzzer.h"
#include "menu.h"
#include "game.h"
#include "debug.h"

unsigned int tweetTimerMillis    = 0;
unsigned int interactTimerMillis = 0;

void welcome()
{
    DEBUG::msg(F("WELCOME"));
    for (int i = 0; i < 10; i++) {
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
    for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
        MEM::writeULong(i * 4, 0);
    }
    DEBUG::msg(F("RESULTS WIPED"));
}

void serviceAction()
{
    DEBUG::msg(F("> SERVICE <"));
    MENU::StartMenu();
    // while (1) {
    //     LED::displayInvalidate();
    //     CONTROL::updateState();
    //     EXIT_FUNCTION_IF_NO_KEY();
    // }
}

void interactWithUserAction()
{
    DEBUG::msg(F("> INTERACT <"));
    while (1) {
        waitCommandBlink(); 
        CONTROL::updateState();

        interactTimerMillis = 0;

        while (
            CONTROL::areAllPressed() &&
            interactTimerMillis <= CONST::TIMELINE_INTERACT_MILLIS
        ) {
            unsigned long lastMillis = millis();
            delay(10);

            LED::progress((int) ((interactTimerMillis / (float) CONST::TIMELINE_INTERACT_MILLIS) * 100));
            LED::displayInvalidate();

            interactTimerMillis += (millis() - lastMillis);
            
            CONTROL::updateState();

            EXIT_FUNCTION_IF_NO_KEY();
        }
        LED::clear();
        if (interactTimerMillis > CONST::TIMELINE_INTERACT_MILLIS) {
            writeInteracted();
            return;
        }
        
        EXIT_FUNCTION_IF_NO_KEY();
    }
}

void showRadiusAction()
{    
    DEBUG::msg(F("> SHOW RADIUS <"));
    int radius = GAME::getRadius();
    
    while (1) {
        delay(CONST::TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS);
        LED::displayNumberBlinking(radius);
        EXIT_FUNCTION_IF_NO_KEY();
    }
}

void printResultTable()
{
    for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
        Serial.print(F("TEAM #"));
        Serial.print(i);
        Serial.print(F(" CHECK-IN: "));
        long checkIn = MEM::readULong(i * 4);
        if (checkIn == 0) {
            Serial.print(F("NONE"));
        } else {
            Serial.print(checkIn);
        }
        Serial.println();
    }
}

void setup()
{
    Serial.begin(115200);
    
    pinMode(CONST::PIN_KEY_WAKE_UP, INPUT);

    pinMode(CONST::PIN_LED_1, OUTPUT);
    pinMode(CONST::PIN_LED_2, OUTPUT);
    pinMode(CONST::PIN_LED_4, OUTPUT);
    pinMode(CONST::PIN_LED_8, OUTPUT);

    pinMode(CONST::PIN_BTN_LEFT, INPUT);
    pinMode(CONST::PIN_BTN_RIGHT, INPUT);

    CONTROL::updateState();
    
    delay(CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS + 50);
    
    CONTROL::updateState();
    if (CONTROL::isBtnRightPressed() && !CONTROL::isBtnLeftPressed()) {
        wipeResults();
        BUZZER::tweet(1000);
    }
    
    DEBUG::log(F("VER"), CONST::VERSION);
    DEBUG::log(F("DEVICE"),  CONST::GAME_DEVICE_ID);
    
    printResultTable();

    BUZZER::notificate();

    attachInterrupt(0, GAME::updateKeyConnectedFlag, CHANGE);
}

void loop()
{
    GAME::updateKeyConnectedFlag();

    if (GAME::isKeyConnected()) {

        delay(CONST::TIMELINE_BEFORE_WELCOME_DELAY_MILLIS);
        welcome();

        while (GAME::isKeyConnected()) {
            if (GAME::isSuper()) {
                serviceAction();
            } else {
                bool interacted = readInteracted();
                if (!interacted) {
                    interactWithUserAction();
                } else {
                    showRadiusAction();
                }
            }
        }
    } else if (tweetTimerMillis >= CONST::TIMELINE_TWEET_DELAY_MILLIS) {
        BUZZER::notificate();
        tweetTimerMillis = 0;
    }

    // going to sleep
    DEBUG::msg(F("SLEEP"));
    LED::clear();
    delay(10);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    DEBUG::msg(F("AWAKEN"));
    // awaken
    tweetTimerMillis += 8000;
}