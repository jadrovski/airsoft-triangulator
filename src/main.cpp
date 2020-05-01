#include <Arduino.h>
#include <LowPower.h>
#include "mem.h"
#include "const.h"
#include "Control.h"
#include <LedDisplay.h>
#include "Buzzer.h"
#include "Game.h"
#include "Debug.h"
//#include "menu.h"

unsigned int tweetTimerMillis    = 0;
unsigned int interactTimerMillis = 0;

Debug debug(CONST::DEBUG);
Buzzer buzzer(CONST::PIN_BUZZER, debug);
Control control(CONST::PIN_BTN_LEFT, CONST::PIN_BTN_RIGHT, CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS, debug);
Game game(
    CONST::PIN_KEY_WAKE_UP,
    CONST::PIN_KEY_ADC,
    CONST::GAME_TEAMS_COUNT,
    CONST::GAME_TEAM_RAW_VALUES,
    CONST::GAME_SUPER_KEY_IDX,
    CONST::GAME_DEVICE_ID,
    CONST::GAME_RADIUSES,
    debug
);
LedDisplay ledDisplay(
    CONST::PIN_LED_1,
    CONST::PIN_LED_2,
    CONST::PIN_LED_4,
    CONST::PIN_LED_8,
    CONST::LED_BLINK_SLOW_DELAY_MILLIS,
    CONST::LED_BLINK_FAST_DELAY_MILLIS,
    game,
    debug
);

void welcome()
{
    debug.msg(F("WELCOME"));
    for (int i = 0; i < 10; i++) {
        if (!game.isKeyConnected()) return;

        ledDisplay.displayNumberBinary(15);
        buzzer.tweet(20);
        ledDisplay.clear();
        delay(20);
    }
}

void waitCommandBlink()
{
    ledDisplay.displayNumberBinary(15);
    delay(10);
    ledDisplay.clear();
    delay(1000);
}

bool readInteracted()
{
    return MEM::readULong(game.getConnectedKeyIdx() * 4) != 0;
}

void writeInteracted()
{
    MEM::writeULong(game.getConnectedKeyIdx() * 4, millis());
}

void wipeResults()
{
    for (auto i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
        MEM::writeULong(i * 4, 0);
    }
    debug.msg(F("RESULTS WIPED"));
}

void serviceAction()
{
    debug.msg(F("> SERVICE <"));
    //MENU::StartMenu();
    // while (1) {
    //     LED::displayInvalidate();
    //     CONTROL::updateState();
    //     EXIT_FUNCTION_IF_NO_KEY();
    // }
}

void interactWithUserAction()
{
    debug.msg(F("> INTERACT <"));
    while (true) {
        waitCommandBlink();
        control.updateState();

        interactTimerMillis = 0;

        while (
            control.areAllPressed() &&
            interactTimerMillis <= CONST::TIMELINE_INTERACT_MILLIS
        ) {
            unsigned long lastMillis = millis();
            delay(10);

            ledDisplay.progress((int) ((interactTimerMillis / (float) CONST::TIMELINE_INTERACT_MILLIS) * 100));
            ledDisplay.displayInvalidate();

            interactTimerMillis += (millis() - lastMillis);

            control.updateState();

            if (!game.isKeyConnected()) return;
        }

        ledDisplay.clear();

        if (interactTimerMillis > CONST::TIMELINE_INTERACT_MILLIS) {
            writeInteracted();
            return;
        }

        if (!game.isKeyConnected()) return;
    }
}

void showRadiusAction()
{
    debug.msg(F("> SHOW RADIUS <"));
    unsigned int radius = game.getRadius();

    while (true) {
        delay(CONST::TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS);
        ledDisplay.displayNumberBlinking(radius);
        if (!game.isKeyConnected()) return;
    }
}

void printResultTable()
{
    for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
        Serial.print(F("TEAM #"));
        Serial.print(i);
        Serial.print(F(" CHECK-IN: "));
        unsigned long checkIn = MEM::readULong(i * 4);
        if (checkIn == 0) {
            Serial.print(F("NONE"));
        } else {
            Serial.print(checkIn);
        }
        Serial.println();
    }
}

void updateKey()
{
    game.updateKeyConnectedFlag();
}

void setup()
{
    Serial.begin(115200);

    pinMode(CONST::PIN_KEY_WAKE_UP, INPUT);

    pinMode(CONST::PIN_LED_1, OUTPUT);
    pinMode(CONST::PIN_LED_2, OUTPUT);
    pinMode(CONST::PIN_LED_4, OUTPUT);
    pinMode(CONST::PIN_LED_8, OUTPUT);

    pinMode(CONST::PIN_BUZZER, OUTPUT);

    pinMode(CONST::PIN_BTN_LEFT, INPUT);
    pinMode(CONST::PIN_BTN_RIGHT, INPUT);

    control.updateState();

    delay(CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS + 50);

    control.updateState();
    if (control.isBtnRightPressed() && !control.isBtnLeftPressed()) {
        wipeResults();
        buzzer.tweet(1000);
    }

    debug.log(F("VER"), CONST::VERSION);
    debug.log(F("DEVICE"), CONST::GAME_DEVICE_ID);

    printResultTable();

    buzzer.notificate();

    attachInterrupt(0, updateKey, CHANGE);
}

void loop()
{
    game.updateKeyConnectedFlag();

    if (game.isKeyConnected()) {

        delay(CONST::TIMELINE_BEFORE_WELCOME_DELAY_MILLIS);
        welcome();

        while (game.isKeyConnected()) {
            if (game.isSuper()) {
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
        buzzer.notificate();
        tweetTimerMillis = 0;
    }

    // going to sleep
    debug.msg(F("SLEEP"));
    ledDisplay.clear();
    delay(10);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    debug.msg(F("AWAKEN"));
    // awaken
    tweetTimerMillis += 8000;
}