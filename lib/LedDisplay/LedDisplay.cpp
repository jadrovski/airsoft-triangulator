#include <Arduino.h>
#include <util.h>
#include <Debug.h>
#include "LedDisplay.h"

LedDisplay::LedDisplay(
    uint8_t pinLed1,
    uint8_t pinLed2,
    uint8_t pinLed4,
    uint8_t pinLed8,
    unsigned int ledBlinkStateDelayMillis,
    unsigned int ledBlinkObviousDelayMillis,
    Debug *debug,
    bool (*exitCondition)()
)
{
    this->_pinLed1 = pinLed1;
    this->_pinLed2 = pinLed2;
    this->_pinLed4 = pinLed4;
    this->_pinLed8 = pinLed8;
    this->_ledBlinkStateDelayMillis   = ledBlinkStateDelayMillis;
    this->_ledBlinkObviousDelayMillis = ledBlinkObviousDelayMillis;
    this->_exitCondition = exitCondition;
    this->_debug = debug;
}

//
//  8  4  2  1   <-- binary
// (x)(x)(x)(x)  <-- leds
//  .  .  .  .
//  l3 l2 l1 l0  <-- args
//
void LedDisplay::setRaw(int l3, int l2, int l1, int l0)
{
    digitalWrite(this->_pinLed1, l0);
    digitalWrite(this->_pinLed2, l1);
    digitalWrite(this->_pinLed4, l2);
    digitalWrite(this->_pinLed8, l3);
}

void LedDisplay::initHardware()
{
    pinMode(this->_pinLed1, OUTPUT);
    pinMode(this->_pinLed2, OUTPUT);
    pinMode(this->_pinLed4, OUTPUT);
    pinMode(this->_pinLed8, OUTPUT);
}

void LedDisplay::clear()
{
    if (this->_debug != nullptr) {
        this->_debug->msg(F("LED CLEAR"));
    }
    setRaw(LOW, LOW, LOW, LOW);
}

void LedDisplay::setState(LED_STATE s0, LED_STATE s1, LED_STATE s2, LED_STATE s3)
{
    l0 = s0;
    l1 = s1;
    l2 = s2;
    l3 = s3;
}

void LedDisplay::setS0(LED_STATE s)
{
    l0 = s;
}

void LedDisplay::setS1(LED_STATE s)
{
    l1 = s;
}

void LedDisplay::setS2(LED_STATE s)
{
    l2 = s;
}

void LedDisplay::setS3(LED_STATE s)
{
    l3 = s;
}

void LedDisplay::displayNumberBinary(unsigned int number)
{
    setRaw(
        IS_BIT_SET(number, 3),
        IS_BIT_SET(number, 2),
        IS_BIT_SET(number, 1),
        IS_BIT_SET(number, 0)
    );
}

void LedDisplay::displayNumberBlinking(unsigned int number)
{
    if (this->_debug != nullptr) {
        this->_debug->log(F("NUMBER"), (int) number);
    }

    unsigned int num3 = (number / 1000) % 10;
    unsigned int num2 = (number / 100 ) % 10;
    unsigned int num1 = (number / 10  ) % 10;
    unsigned int num0 = (number / 1   ) % 10;

    while (num0 > 0 || num1 > 0 || num2 > 0 || num3 > 0)
    {
        if (_exitCondition != nullptr && _exitCondition()) {
            return;
        }

        setRaw(
            num3 > 0 ? HIGH : LOW,
            num2 > 0 ? HIGH : LOW,
            num1 > 0 ? HIGH : LOW,
            num0 > 0 ? HIGH : LOW
        );

        delay(_ledBlinkObviousDelayMillis);

        clear();

        delay(_ledBlinkObviousDelayMillis);

        num0--; num1--; num2--; num3--;
    }

    clear();
}

void LedDisplay::displayInvalidate()
{
    if (l0 != l0_prev || l1 != l1_prev || l2 != l2_prev || l3 != l3_prev) {
        l0_prev = l0;
        l1_prev = l1;
        l2_prev = l2;
        l3_prev = l3;

        if (l0 == BLINK_ON || l0 == ON) {
            l0_val = HIGH;
        } else {
            l0_val = LOW;
        }

        if (l1 == BLINK_ON || l1 == ON) {
            l1_val = HIGH;
        } else {
            l1_val = LOW;
        }

        if (l2 == BLINK_ON || l2 == ON) {
            l2_val = HIGH;
        } else {
            l2_val = LOW;
        }

        if (l3 == BLINK_ON || l3 == ON) {
            l3_val = HIGH;
        } else {
            l3_val = LOW;
        }
    }

    // switching blinks
    if (
        l0 == BLINK_OFF || l0 == BLINK_ON
            ||
        l1 == BLINK_OFF || l1 == BLINK_ON
            ||
        l2 == BLINK_OFF || l2 == BLINK_ON
            ||
        l3 == BLINK_OFF || l3 == BLINK_ON
    ) {
        unsigned long currentMillis = millis();
        if ((currentMillis - _ledLastBlinkMillis) >= _ledBlinkStateDelayMillis) {
            _ledLastBlinkMillis = currentMillis;
            //invert blinks!

            if (l0 == BLINK_ON || l0 == BLINK_OFF) {
                l0_val = !l0_val;
            }
            if (l1 == BLINK_ON || l1 == BLINK_OFF) {
                l1_val = !l1_val;
            }
            if (l2 == BLINK_ON || l2 == BLINK_OFF) {
                l2_val = !l2_val;
            }
            if (l3 == BLINK_ON || l3 == BLINK_OFF) {
                l3_val = !l3_val;
            }
        }
    }

    setRaw(l3_val, l2_val, l1_val, l0_val);
}

void LedDisplay::progress(int percentage)
{
    if (this->_debug != nullptr) {
        this->_debug->log(F("PROGRESS"), percentage);
    }

    LED_STATE s0 = LED_STATE::OFF;
    LED_STATE s1 = LED_STATE::OFF;
    LED_STATE s2 = LED_STATE::OFF;
    LED_STATE s3 = LED_STATE::OFF;

    if (percentage >= 0 && percentage < 25) {
        s3 = LED_STATE::BLINK_ON;
    } else if (percentage >= 25 && percentage < 50) {
        s3 = LED_STATE::ON;
        s2 = LED_STATE::BLINK_ON;
    } else if (percentage >= 50 && percentage < 75) {
        s3 = LED_STATE::ON;
        s2 = LED_STATE::ON;
        s1 = LED_STATE::BLINK_ON;
    } else if (percentage >= 75 && percentage < 100) {
        s3 = LED_STATE::ON;
        s2 = LED_STATE::ON;
        s1 = LED_STATE::ON;
        s0 = LED_STATE::BLINK_ON;
    } else if (percentage >= 100) {
        s3 = LED_STATE::ON;
        s2 = LED_STATE::ON;
        s1 = LED_STATE::ON;
        s0 = LED_STATE::ON;
    }

    setState(s0, s1, s2, s3);
}