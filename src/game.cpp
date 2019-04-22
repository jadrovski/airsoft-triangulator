#include <Arduino.h>
#include "const.h"
#include "debug.h"

namespace GAME {
    
    volatile bool isKeyConnectedFlag = false;
    volatile int  connectedKeyIdx    = -1;
    
    void updateKeyConnectedFlag()
    {
        isKeyConnectedFlag = digitalRead(CONST::PIN_KEY_WAKE_UP);
        if (!isKeyConnectedFlag) {
            connectedKeyIdx = -1;
        }
    }

    int getConnectedKeyIdx()
    {
        if (!isKeyConnectedFlag) {
            return -1;
        }

        if (connectedKeyIdx == -1) {
            int adcRaw = analogRead(CONST::PIN_KEY_ADC);

            DEBUG::log("RAW VALUE KEY", adcRaw);
            
            if (adcRaw) {
                for (unsigned int i = 0; i < CONST::GAME_TEAMS_COUNT; i++) {
                    if (abs(adcRaw - CONST::GAME_TEAM_RAW_VALUES[i]) < 10) {
                        connectedKeyIdx = i;
                        DEBUG::log("CONNECTED KEY", connectedKeyIdx);
                        break;
                    }
                }
            }
        }

        return connectedKeyIdx;
    }
    
    bool isKeyConnected()
    {
        return isKeyConnectedFlag && getConnectedKeyIdx() != -1;
    }
    
    bool isSuper()
    {
        return isKeyConnectedFlag && getConnectedKeyIdx() == CONST::GAME_SUPER_KEY_IDX;
    }
    
    int getRadius()
    {
        return CONST::GAME_RADIUSES[CONST::GAME_DEVICE_ID - 1];
    }
}