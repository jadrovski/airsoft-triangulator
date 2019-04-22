#include <Arduino.h>
#include "const.h"
#include "MelodyUtils.h"
#include "pitches.h"
#include "debug.h"

namespace BUZZER {
    void tweet(unsigned long ms)
    {
        DEBUG::log("TWEET", ms);
        tone(CONST::PIN_BUZZER, random(300, 1500));
        delay(ms);
        noTone(CONST::PIN_BUZZER);
    }

    MelodyUtils mel(CONST::PIN_BUZZER);
    
    void wtf()
    {
        mel.Glis(NOTE_C3, NOTE_C4, 2);
        delay(100);
        mel.Glis(NOTE_C3, NOTE_C4, 2);
        delay(100);
        mel.Glis(NOTE_C3, NOTE_C4, 2);
        delay(100);
        mel.Trem(NOTE_C3, 1000, 30);
    }

    void notificate()
    {
        for (int i = 1000; i <= 3000; i++) {
            tone(CONST::PIN_BUZZER, i);
            delay(50);
            noTone(CONST::PIN_BUZZER);
            i+=400;
        }
    }
}