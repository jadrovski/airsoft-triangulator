#include <Arduino.h>
#include <LowPower.h>
#include <mem.h>
#include "TriangulatorGame.h"

#define EXIT_IF_NO_KEY() if (!_key.isConnected()) return

TriangulatorGame::TriangulatorGame(
    uint8_t         deviceId,
    const uint16_t* radiuses,
    Buzzer          &buzzer,
    Control         &control,
    Key             &key,
    LedDisplay      &ledDisplay,
    Debug           &debug
): _buzzer(buzzer), _control(control), _key(key), _ledDisplay(ledDisplay), _debug(debug)
{
    this->_deviceId      = deviceId;
    this->_radiuses      = radiuses;
}

unsigned int TriangulatorGame::getRadius()
{
    return this->_radiuses[this->_deviceId - 1];
}

void TriangulatorGame::welcome()
{
    this->_debug.msg(F("WELCOME"));
    for (uint8_t i = 0; i < 10; i++) {
        EXIT_IF_NO_KEY();

        this->_ledDisplay.displayNumberBinary(15);
        this->_buzzer.tweet(20);
        this->_ledDisplay.clear();
        delay(20);
    }
}

bool TriangulatorGame::readInteracted()
{
    return MEM::readULong(this->_key.getConnectedKeyIdx() * 4) != 0;
}

void TriangulatorGame::writeInteracted()
{
    MEM::writeULong(this->_key.getConnectedKeyIdx() * 4, millis());
}

void TriangulatorGame::waitCommandBlink()
{
    this->_ledDisplay.displayNumberBinary(15);
    delay(10);
    this->_ledDisplay.clear();
    delay(1000);
}

void TriangulatorGame::interactWithUserAction()
{
    this->_debug.msg(F("> INTERACT <"));
    while (true) {
        waitCommandBlink();
        this->_control.updateState();

        _interactTimerMillis = 0;

        while (
            this->_control.areAllPressed() &&
            _interactTimerMillis <= TIMELINE_INTERACT_MILLIS
        ) {
            unsigned long lastMillis = millis();
            delay(10);

            this->_ledDisplay.progress((int) (((float) _interactTimerMillis / (float) TIMELINE_INTERACT_MILLIS) * 100));
            this->_ledDisplay.displayInvalidate();

            _interactTimerMillis += (millis() - lastMillis);

            this->_control.updateState();

            if (!this->_key.isConnected()) return;
        }

        this->_ledDisplay.clear();

        if (_interactTimerMillis > TIMELINE_INTERACT_MILLIS) {
            writeInteracted();
            return;
        }

        if (!this->_key.isConnected()) return;
    }
}

void TriangulatorGame::showRadiusAction()
{
    this->_debug.msg(F("> SHOW RADIUS <"));
    unsigned int radius = getRadius();

    while (true) {
        delay(TIMELINE_DELAY_BETWEEN_DISPLAY_NUMBER_MILLIS);
        this->_ledDisplay.displayNumberBlinking(radius);
        EXIT_IF_NO_KEY();
    }
}

void TriangulatorGame::wipeResults()
{
    for (auto i = 0; i < this->_key.getTeamsCount(); i++) {
        MEM::writeULong(i * 4, 0);
    }
    this->_debug.msg(F("RESULTS WIPED"));
}

void TriangulatorGame::printResultTable()
{
    for (unsigned int i = 0; i < this->_key.getTeamsCount(); i++) {
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

void TriangulatorGame::init()
{
    this->_control.updateState();
    delay(Control::TIMELINE_CONTROL_DEBOUNCE_MILLIS + 50);
    this->_control.updateState();
    if (this->_control.isBtnRightPressed() && !this->_control.isBtnLeftPressed()) {
        wipeResults();
        this->_buzzer.tweet(1000);
    }

    this->_debug.log(F("DEVICE"), this->_deviceId);

    this->printResultTable();

    this->_buzzer.notificate();
}

void TriangulatorGame::run()
{
    this->_key.updateConnectedFlag();

    if (this->_key.isConnected()) {

        delay(TIMELINE_BEFORE_WELCOME_DELAY_MILLIS);
        welcome();

        while (this->_key.isConnected()) {
            if (this->_key.isSuper()) {
                //@todo service action?
            } else {
                bool interacted = readInteracted();
                if (!interacted) {
                    interactWithUserAction();
                } else {
                    showRadiusAction();
                }
            }
        }
    } else if (_tweetTimerMillis >= TIMELINE_TWEET_DELAY_MILLIS) {
        this->_buzzer.notificate();
        _tweetTimerMillis = 0;
    }

    // going to sleep
    this->_debug.msg(F("SLEEP"));
    this->_ledDisplay.clear();
    delay(10);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

    this->_debug.msg(F("AWAKEN"));
    // awaken
    _tweetTimerMillis += 8000;
}
