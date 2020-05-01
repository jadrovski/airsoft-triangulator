#ifndef BUZZER_H_
#define BUZZER_H_

#include <Arduino.h>
#include "const.h"
//#include "MelodyUtils.h"
//#include "pitches.h"
#include "Debug.h"

class Buzzer {
public:
    Buzzer(uint8_t pin, Debug &debug);
    void beep();
    void tweet(unsigned long ms);
    void notificate();
private:
    uint8_t _pin;
    Debug &_debug;
};

#endif
