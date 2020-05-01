#include <Arduino.h>
#include "Debug.h"
#include "Game.h"

Game::Game(
    uint8_t    pinKeyWakeUp,
    uint8_t    pinKeyAdc,
    uint8_t    teamsCount,
    const int* teamRawValues,
    uint8_t    superKeyIdx,
    uint8_t    deviceId,
    const unsigned int* radiuses,
    Debug      &debug
): _debug(debug)
{
    this->_pinKeyWakeUp  = pinKeyWakeUp;
    this->_pinKeyAdc     = pinKeyAdc;
    this->_teamsCount    = teamsCount;
    this->_teamRawValues = teamRawValues;
    this->_superKeyIdx   = superKeyIdx;
    this->_deviceId      = deviceId;
    this->_radiuses      = radiuses;
}

void Game::updateKeyConnectedFlag()
{
    this->isKeyConnectedFlag = digitalRead(this->_pinKeyWakeUp);
    if (!this->isKeyConnectedFlag) {
        this->connectedKeyIdx = -1;
    }
}

int Game::getConnectedKeyIdx()
{
    if (!this->isKeyConnectedFlag) {
        return -1;
    }

    if (this->connectedKeyIdx == -1) {
        int adcRaw = analogRead(this->_pinKeyAdc);

        this->_debug.log(F("RAW KEY"), adcRaw);

        if (adcRaw) {
            for (unsigned int i = 0; i < this->_teamsCount; i++) {
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

bool Game::isKeyConnected()
{
    return this->isKeyConnectedFlag && this->getConnectedKeyIdx() != -1;
}

bool Game::isSuper()
{
    return this->isKeyConnectedFlag && this->getConnectedKeyIdx() == this->_superKeyIdx;
}

unsigned int Game::getRadius()
{
    return this->_radiuses[this->_deviceId - 1];
}
