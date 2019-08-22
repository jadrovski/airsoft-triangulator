#include <Arduino.h>
#include "const.h"
#include "util.h"
#include "game.h"
#include "led_display.h"
#include "debug.h"

namespace LED {

    const unsigned int LED_BLINK_STATE_DELAY_MILLIS   = CONST::LED_BLINK_FAST_DELAY_MILLIS;
    const unsigned int LED_BLINK_OBVIOUS_DELAY_MILLIS = CONST::LED_BLINK_SLOW_DELAY_MILLIS;

    unsigned long ledLastBlinkMillis = 0;

    LED_STATE l3 = LED_STATE::OFF;
    LED_STATE l2 = LED_STATE::OFF;
    LED_STATE l1 = LED_STATE::OFF;
    LED_STATE l0 = LED_STATE::OFF;

    LED_STATE l3_prev = LED_STATE::OFF;
    LED_STATE l2_prev = LED_STATE::OFF;
    LED_STATE l1_prev = LED_STATE::OFF;
    LED_STATE l0_prev = LED_STATE::OFF;

    int l3_val = LOW;
    int l2_val = LOW;
    int l1_val = LOW;
    int l0_val = LOW;

    //
    //  8  4  2  1   <-- binary
    // (x)(x)(x)(x)  <-- leds 
    //  .  .  .  .
    //  l3 l2 l1 l0  <-- args
    //
    void setRaw(int l3, int l2, int l1, int l0)
    {
        digitalWrite(CONST::PIN_LED_1, l0);
        digitalWrite(CONST::PIN_LED_2, l1);
        digitalWrite(CONST::PIN_LED_4, l2);
        digitalWrite(CONST::PIN_LED_8, l3);
    }

    void clear()
    {
        DEBUG::msg(F("LED CLEAR"));
        setRaw(LOW, LOW, LOW, LOW);
    }

    void setState(LED_STATE s0, LED_STATE s1, LED_STATE s2, LED_STATE s3)
    {
        l0 = s0;
        l1 = s1;
        l2 = s2;
        l3 = s3;
    }

    void setS0(LED_STATE s)
    {
        l0 = s;
    }

    void setS1(LED_STATE s)
    {
        l1 = s;
    }

    void setS2(LED_STATE s)
    {
        l2 = s;
    }

    void setS3(LED_STATE s)
    {
        l3 = s;
    }

    void displayNumberBinary(int number)
    {
        setRaw(
            IS_BIT_SET(number, 3),
            IS_BIT_SET(number, 2),
            IS_BIT_SET(number, 1),
            IS_BIT_SET(number, 0)
        );
    }

    void displayNumberBlinking(int number)
    {
        DEBUG::log(F("NUMBER"), number);

        int num3 = (number / 1000) % 10;
        int num2 = (number / 100 ) % 10;
        int num1 = (number / 10  ) % 10;
        int num0 = (number / 1   ) % 10;

        while (num0 > 0 || num1 > 0 || num2 > 0 || num3 > 0)
        {
            EXIT_FUNCTION_IF_NO_KEY();

            setRaw(
                num3 > 0 ? HIGH : LOW,
                num2 > 0 ? HIGH : LOW,
                num1 > 0 ? HIGH : LOW,
                num0 > 0 ? HIGH : LOW
            );

            delay(LED_BLINK_OBVIOUS_DELAY_MILLIS);
            
            clear();

            delay(LED_BLINK_OBVIOUS_DELAY_MILLIS);

            num0--; num1--; num2--; num3--;
        }

        clear();
    }

    void displayInvalidate()
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
            if ((currentMillis - ledLastBlinkMillis) >= LED_BLINK_STATE_DELAY_MILLIS) {
                ledLastBlinkMillis = currentMillis;
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

    void progress(int percentage)
    {
        DEBUG::log(F("PROGRESS"), percentage);
    
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
}