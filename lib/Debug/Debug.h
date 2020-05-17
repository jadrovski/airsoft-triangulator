#ifndef NEPOPAL_DEBUG_H_
#define NEPOPAL_DEBUG_H_

#include <Arduino.h>

class Debug {
public:
    Debug(bool enabled = true);
    void log(const __FlashStringHelper *param, int value);
    void msg(const __FlashStringHelper *msg);

private:
    bool _enabled = false;
};

#endif
