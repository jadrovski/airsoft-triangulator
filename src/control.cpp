#include <Arduino.h>
#include "const.h"
#include "debug.h"

namespace CONTROL {
    int btnRightState;
    int btnLeftState;
    int lastBtnRightState;
    int lastBtnLeftState;

    unsigned long lastDebounceTimeBtnRight = 0;
    unsigned long lastDebounceTimeBtnLeft  = 0;
    
    bool isBtnRightPressed()
    {
        return btnRightState == HIGH;
    }
    
    bool isBtnLeftPressed()
    {
        return btnLeftState == HIGH;
    }
    
    bool areAllPressed()
    {
        return isBtnRightPressed() && isBtnLeftPressed();
    }

    void updateState()
    {
        int btnLeft  = digitalRead(CONST::PIN_BTN_LEFT);
        int btnRight = digitalRead(CONST::PIN_BTN_RIGHT);

        if (btnLeft != lastBtnLeftState) {
            lastDebounceTimeBtnLeft = millis();
        }

        if (btnRight != lastBtnRightState) {
            lastDebounceTimeBtnRight = millis();
        }

        if ((millis() - lastDebounceTimeBtnLeft) > CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS && btnLeft != btnLeftState) {
            btnLeftState = btnLeft;
            DEBUG::log("BUTTON LEFT  ", btnLeft);
        }

        if ((millis() - lastDebounceTimeBtnRight) > CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS && btnRight != btnRightState) {
            btnRightState = btnRight;
            DEBUG::log("BUTTON RIGHT ", btnRight);
        }

        lastBtnLeftState  = btnLeft;
        lastBtnRightState = btnRight;
    }
}