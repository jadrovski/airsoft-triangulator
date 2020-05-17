#include <Arduino.h>
#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin, Debug &debug): _debug(debug)
{
    this->_pin = pin;
}

void Buzzer::tweet(unsigned long ms) {
    this->_debug.log(F("TWEET"), ms);
    //tone(this->_pin, random(300, 1500));
    delay(ms);
    //noTone(this->_pin);
}

void Buzzer::notificate() {
    this->_debug.msg(F("NOTIFICATE"));
    for (int i = 1000; i <= 3000; i++) {
        //tone(this->_pin, i);
        delay(50);
        //noTone(this->_pin);
        i += 400;
    }
}

void Buzzer::initHardware()
{
    pinMode(this->_pin, OUTPUT);
}

// MelodyUtils mel(CONST::PIN_BUZZER);

// void wtf()
// {
//     mel.Glis(NOTE_C3, NOTE_C4, 2);
//     delay(100);
//     mel.Glis(NOTE_C3, NOTE_C4, 2);
//     delay(100);
//     mel.Glis(NOTE_C3, NOTE_C4, 2);
//     delay(100);
//     mel.Trem(NOTE_C3, 1000, 30);
// }