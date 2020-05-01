#ifndef GAME_H_
#define GAME_H_

#include <Arduino.h>
#include "Debug.h"

class Game {
public:
    Game(
        uint8_t    pinKeyWakeUp,
        uint8_t    pinKeyAdc,
        uint8_t    teamsCount,
        const int* teamRawValues,
        uint8_t    superKeyIdx,
        uint8_t    deviceId,
        const unsigned int* radiuses,
        Debug      &debug
    );
    bool isKeyConnected();
    void updateKeyConnectedFlag();
    int  getConnectedKeyIdx();
    bool isSuper();
    unsigned int getRadius();
private:
    volatile bool isKeyConnectedFlag = false;
    volatile int  connectedKeyIdx    = -1;
    uint8_t _pinKeyWakeUp;
    uint8_t _pinKeyAdc;
    uint8_t _teamsCount;
    const int* _teamRawValues;
    uint8_t _superKeyIdx;
    uint8_t _deviceId;
    const unsigned int* _radiuses;
    Debug &_debug;
};

#endif
