#include <Arduino.h>
#include "const.h"

namespace CONTROL {
    int btn1State;
    int btn2State;
    int lastBtn1State;
    int lastBtn2State;

    unsigned long lastDebounceTimeBtn1 = 0;
    unsigned long lastDebounceTimeBtn2 = 0;
    
    bool isBtn1Pressed()
    {
        return btn1State == HIGH;
    }
    
    bool isBtn2Pressed()
    {
        return btn2State == HIGH;
    }
    
    bool areAllPressed()
    {
        return isBtn1Pressed() && isBtn2Pressed();
    }

    void updateState()
    {
        int btn1 = digitalRead(CONST::PIN_BTN_1);
        int btn2 = digitalRead(CONST::PIN_BTN_2);

        if (btn1 != lastBtn1State) 
        {
            lastDebounceTimeBtn1 = millis();
        }
        if (btn2 != lastBtn2State)
        {
            lastDebounceTimeBtn2 = millis();
        }

        if ((millis() - lastDebounceTimeBtn1) > CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS)
        {
            if (btn1 != btn1State)
            {
                btn1State = btn1;
            }
        }
        if ((millis() - lastDebounceTimeBtn2) > CONST::TIMELINE_CONTROL_DEBOUNCE_MILLIS)
        {
            if (btn2 != btn2State)
            {
                btn2State = btn2;
            }
        }

        lastBtn1State = btn1;
        lastBtn2State = btn2;
    }
}