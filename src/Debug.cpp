#include <Arduino.h>
#include "Debug.h"

uint8_t _enabled = false;

Debug::Debug(bool enabled) {
    _enabled = enabled;
}

void Debug::log(const __FlashStringHelper* param, int value)
{
    if (_enabled) {
        Serial.print(param);
        Serial.print(": ");
        Serial.println(value);
    }
}

void Debug::msg(const __FlashStringHelper *msg)
{
    if (_enabled) {
        Serial.println(msg);
    }
}