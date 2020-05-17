#ifndef NEPOPAL_TRIANGULATOR_CONST_H_
#define NEPOPAL_TRIANGULATOR_CONST_H_

namespace CONST {
    const bool    DEBUG                  = true;
    const uint8_t VERSION                = 1;

    const uint8_t  GAME_DEVICE_ID         = 3; // 1, 2, 3
    const uint16_t GAME_RADIUSES[]        = {305, 511, 362};

    const uint8_t  KEY_SUPER_IDX         = 0;
    const uint8_t  KEY_TEAMS_COUNT       = 5;
    const uint16_t KEY_TEAM_RAW_VALUES[] = {
        415, //  15kOhm idx = 0 # SUPER 
        205, //  39kOhm idx = 1
        130, //  68kOhm idx = 2
        100, //  91kOhm idx = 3
        78   // 120kOhm idx = 4
    };
    
    const uint8_t PIN_LED_1       = 5;
    const uint8_t PIN_LED_2       = 6;
    const uint8_t PIN_LED_4       = 4;
    const uint8_t PIN_LED_8       = 8;
    const uint8_t PIN_BTN_LEFT    = 13;
    const uint8_t PIN_BTN_RIGHT   = 12;
    const uint8_t PIN_KEY_ADC     = 0;
    const uint8_t PIN_KEY_WAKE_UP = 2;
    const uint8_t PIN_BUZZER      = 10;
}

#endif
