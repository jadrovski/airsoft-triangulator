#include <Arduino.h>
#include "const.h"

namespace BUZZER {
    void tweet(unsigned long ms)
    {
        tone(CONST::PIN_BUZZER, random(300, 1500));
        delay(ms);
        noTone(CONST::PIN_BUZZER);
    }
}
