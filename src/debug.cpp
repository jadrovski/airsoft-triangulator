#include <Arduino.h>
#include "const.h"

namespace DEBUG {
    void log(const __FlashStringHelper* param, int value)
    {
        if (CONST::DEBUG) {
            Serial.print(param);
            Serial.print(": ");
            Serial.println(value);
        }
    }

    void msg(const __FlashStringHelper *msg)
    {
        if (CONST::DEBUG) {
            Serial.println(msg);
        }
    }
}