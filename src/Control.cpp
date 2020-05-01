#include <Arduino.h>
#include "Debug.h"
#include "Control.h"

Control::Control(uint8_t pinBtnLeft, uint8_t pinBtnRight, unsigned int debounceMillis, Debug &debug): _debug(debug)
{
    this->_pinBtnLeft  = pinBtnLeft;
    this->_pinBtnRight = pinBtnRight;
    this->_debounceMillis = debounceMillis;
}

bool Control::isBtnRightPressed() {
    return _btnRightState == HIGH;
}

bool Control::isBtnLeftPressed() {
    return _btnLeftState == HIGH;
}

bool Control::areAllPressed() {
    return isBtnRightPressed() && isBtnLeftPressed();
}

void Control::updateState() {
    int btnLeft  = digitalRead(this->_pinBtnLeft);
    int btnRight = digitalRead(this->_pinBtnRight);

    if (btnLeft != this->_lastBtnLeftState) {
        this->_lastDebounceTimeBtnLeft = millis();
    }

    if (btnRight != this->_lastBtnRightState) {
        this->_lastDebounceTimeBtnRight = millis();
    }

    if ((millis() - this->_lastDebounceTimeBtnLeft) > this->_debounceMillis && btnLeft != this->_btnLeftState) {
        _btnLeftState = btnLeft;
        this->_debug.log(F("BTN L"), btnLeft);
    }

    if ((millis() - this->_lastDebounceTimeBtnRight) > this->_debounceMillis &&
        btnRight != this->_btnRightState) {
        this->_btnRightState = btnRight;
        this->_debug.log(F("BTN R"), btnRight);
    }

    this->_lastBtnLeftState  = btnLeft;
    this->_lastBtnRightState = btnRight;
}