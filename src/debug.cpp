#include <Arduino.h>
#include "const.h"

namespace DEBUG {
    void log(const char param[], int value)
    {
        if (CONST::DEBUG) {
            Serial.print(param);
            Serial.print(": ");
            Serial.println(value);
        }
    }
}