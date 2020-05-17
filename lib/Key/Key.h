#ifndef NEPOPAL_KEY_H
#define NEPOPAL_KEY_H

#include <Arduino.h>
#include <AbstractHardware.h>

class Key : public AbstractHardware {
public:
    Key(
        uint8_t    pinKeyWakeUp,
        uint8_t    pinKeyAdc,
        uint8_t    teamsCount,
        const uint16_t* teamRawValues,
        uint8_t    superKeyIdx,
        Debug      &debug
    );
    void initHardware() override;
    uint8_t getTeamsCount();
    bool isConnected();
    void updateConnectedFlag();
    int  getConnectedKeyIdx();
    bool isSuper();
private:
    volatile bool isKeyConnectedFlag = false;
    volatile int  connectedKeyIdx    = -1;
    uint8_t _pinKeyWakeUp;
    uint8_t _pinKeyAdc;
    uint8_t _teamsCount;
    const uint16_t* _teamRawValues;
    uint8_t _superKeyIdx;
    Debug &_debug;
};

#endif