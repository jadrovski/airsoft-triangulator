#include <Arduino.h>
#include <Debug.h>
#include "Key.h"

Key::Key(
    uint8_t    pinKeyWakeUp, //@todo restrict to pin 2 or 3
    uint8_t    pinKeyAdc,
    uint8_t    teamsCount,
    const uint16_t* teamRawValues,
    uint8_t    superKeyIdx,
    Debug      &debug
): _debug(debug)
{
    this->_pinKeyWakeUp  = pinKeyWakeUp;
    this->_pinKeyAdc     = pinKeyAdc;
    this->_teamsCount    = teamsCount;
    this->_teamRawValues = teamRawValues;
    this->_superKeyIdx   = superKeyIdx;
}

uint8_t Key::getTeamsCount()
{
    return this->_teamsCount;
}

void Key::updateConnectedFlag()
{
    this->isKeyConnectedFlag = digitalRead(this->_pinKeyWakeUp);
    if (!this->isKeyConnectedFlag) {
        this->connectedKeyIdx = -1;
    }
}

int Key::getConnectedKeyIdx()
{
    if (!this->isKeyConnectedFlag) {
        return -1;
    }

    if (this->connectedKeyIdx == -1) {
        int adcRaw = analogRead(this->_pinKeyAdc);

        this->_debug.log(F("RAW KEY"), adcRaw);

        if (adcRaw) {
            for (uint8_t i = 0; i < this->_teamsCount; i++) {
                if (abs(adcRaw - this->_teamRawValues[i]) < 10) {
                    this->connectedKeyIdx = i;
                    this->_debug.log(F("CONNECTED KEY"), this->connectedKeyIdx);
                    break;
                }
            }
        }
    }

    return this->connectedKeyIdx;
}

bool Key::isConnected()
{
    return this->isKeyConnectedFlag && this->getConnectedKeyIdx() != -1;
}

bool Key::isSuper()
{
    return this->isConnected() && this->getConnectedKeyIdx() == this->_superKeyIdx;
}

void Key::initHardware() {
    pinMode(this->_pinKeyWakeUp, INPUT);
}
