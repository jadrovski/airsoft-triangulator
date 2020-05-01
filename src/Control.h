#ifndef CONTROL_H_
#define CONTROL_H_

#include <Arduino.h>
#include "Debug.h"

class Control {
public:
    Control(uint8_t pinBtnLeft, uint8_t pinBtnRight, unsigned int debounceMillis, Debug &debug);
    void updateState();
    bool isBtnLeftPressed();
    bool isBtnRightPressed();
    bool areAllPressed();
private:
    int _btnRightState;
    int _btnLeftState;
    int _lastBtnRightState;
    int _lastBtnLeftState;

    unsigned long _lastDebounceTimeBtnRight = 0;
    unsigned long _lastDebounceTimeBtnLeft  = 0;

    uint8_t _pinBtnRight;
    uint8_t _pinBtnLeft;
    Debug &_debug;
    unsigned long _debounceMillis;
};

#endif
