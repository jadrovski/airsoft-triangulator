#include <Arduino.h>
#include "pin.h"

namespace BUZZER {
    void tweet(unsigned long ms)
    {
        tone(PIN::BUZZER, random(300, 1500));
        delay(ms);
        noTone(PIN::BUZZER);
    }
}