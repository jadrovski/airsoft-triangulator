#ifndef CONST_H_
#define CONST_H_

namespace CONST {
    const bool         DEBUG                  = true;
    const unsigned int VERSION                = 1;
    const unsigned int GAME_DEVICE_ID         = 1; // 1, 2, 3
    const unsigned int GAME_RADIUSES[]        = {750, 300, 495};
    
    const unsigned int GAME_TEAMS_COUNT       = 5;
    const          int GAME_TEAM_RAW_VALUES[] = {
        415, //  15kOhm idx = 0 # SUPER 
        205, //  39kOhm idx = 1
        130, //  68kOhm idx = 2
        100, //  91kOhm idx = 3
        78   // 120kOhm idx = 4
    };
    const unsigned int GAME_SUPER_KEY_IDX     = 0;
    
    const unsigned int PIN_LED_1       = 5;
    const unsigned int PIN_LED_2       = 6;
    const unsigned int PIN_LED_4       = 4;
    const unsigned int PIN_LED_8       = 8;
    const unsigned int PIN_BTN_LEFT    = 13;
    const unsigned int PIN_BTN_RIGHT   = 12;
    const unsigned int PIN_KEY_ADC     = 0;
    const unsigned int PIN_KEY_WAKE_UP = 2;
    const unsigned int PIN_BUZZER      = 10;
    
    const unsigned int TIMELINE_BEFORE_WELCOME_DELAY_MILLIS         = 500;
    const unsigned int TIMELINE_TWEET_DELAY_MILLIS                  = 60000; // Can't be lower than SLEEP_8S
    const unsigned int TIMELINE_INTERACT_MILLIS                     = 60000;
    const unsigned int TIMELINE_CONTROL_DEBOUNCE_MILLIS             = 100;
    const unsigned int TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS = 2000;
}

#endif
