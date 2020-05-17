#ifndef NEPOPAL_TRIANGULATOR_GAME_H_
#define NEPOPAL_TRIANGULATOR_GAME_H_

#include <Arduino.h>
#include <Buzzer.h>
#include <Control.h>
#include <Key.h>
#include <LedDisplay.h>
#include <Debug.h>

class TriangulatorGame {
public:
    TriangulatorGame(
        uint8_t      deviceId,
        const uint16_t* radiuses,
        Buzzer       &buzzer,
        Control      &control,
        Key          &key,
        LedDisplay   &ledDisplay,
        Debug        &debug
    );
    unsigned int getRadius();
    void run();
    void init();
private:
    static const unsigned int TIMELINE_BEFORE_WELCOME_DELAY_MILLIS         = 500;
    static const unsigned int TIMELINE_TWEET_DELAY_MILLIS                  = 60000; // Can't be lower than SLEEP_8S
    static const unsigned int TIMELINE_INTERACT_MILLIS                     = 60000;
    static const unsigned int TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS = 2000;

    void welcome();
    void waitCommandBlink();
    void interactWithUserAction();
    void showRadiusAction();
    void writeInteracted();
    bool readInteracted();
    void wipeResults();
    void printResultTable();

    uint8_t _deviceId;
    const unsigned int* _radiuses;
    Buzzer       &_buzzer;
    Control      &_control;
    Key          &_key;
    LedDisplay   &_ledDisplay;
    Debug        &_debug;
    unsigned int _tweetTimerMillis    = 0;
    unsigned int _interactTimerMillis = 0;
};

#endif
