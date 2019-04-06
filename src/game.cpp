#include <Arduino.h>
#include "const.h"
#include "debug.h"

namespace GAME {
    volatile bool isKeyConnectedFlag = false;
    
    void updateKeyConnectedFlag()
    {
        isKeyConnectedFlag = digitalRead(CONST::PIN_KEY_WAKE_UP);
    }

    int getConnectedKeyIdx()
    {
        if (!isKeyConnectedFlag) {
            return -1;
        }

        int adcRaw = analogRead(CONST::PIN_KEY_ADC);
        
        //DEBUG::log("RAW KEY", adcRaw);
        
        if (adcRaw) {
            for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
                if (abs(adcRaw - CONST::GAME_TEAM_RAW_VALUES[i]) < 10) {
                    return i;
                }
            }
        }

        return -1;
    }
    
    bool isKeyConnected()
    {
        return isKeyConnectedFlag && getConnectedKeyIdx() != -1;
    }
    
    bool isSuper()
    {
        return getConnectedKeyIdx() == CONST::GAME_SUPER_KEY_IDX;
    }
    
    int getRadius()
    {
        return CONST::GAME_RADIUSES[CONST::GAME_DEVICE_ID - 1];
    }
}