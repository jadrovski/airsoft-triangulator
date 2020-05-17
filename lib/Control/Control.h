#ifndef NEPOPAL_CONTROL_H_
#define NEPOPAL_CONTROL_H_

#include <Arduino.h>
#include <Debug.h>
#include <AbstractHardware.h>

class Control : public AbstractHardware {
public:
    static const unsigned int TIMELINE_CONTROL_DEBOUNCE_MILLIS;

    Control(uint8_t pinBtnLeft, uint8_t pinBtnRight, unsigned int debounceMillis, Debug &debug);
    void initHardware() override;
    void updateState();
    bool isBtnLeftPressed();
    bool isBtnRightPressed();
    bool areAllPressed();
private:
    //@todo undefined state
    int _btnRightState = -1;
    int _btnLeftState = -1;
    int _lastBtnRightState = -1;
    int _lastBtnLeftState = -1;

    unsigned long _lastDebounceTimeBtnRight = 0;
    unsigned long _lastDebounceTimeBtnLeft  = 0;

    uint8_t _pinBtnRight;
    uint8_t _pinBtnLeft;
    Debug &_debug;
    unsigned long _debounceMillis;
};

#endif
