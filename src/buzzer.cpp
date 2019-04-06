#include <Arduino.h>
#include "const.h"
#include "MelodyUtils.h"
#include "pitches.h"

namespace BUZZER {
    void tweet(unsigned long ms)
    {
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
}