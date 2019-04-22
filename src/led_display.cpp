#include <Arduino.h>
#include "const.h"
#include "util.h"
#include "game.h"
#include "led_display.h"
#include "debug.h"

namespace LED {

    const unsigned int LED_BLINK_DELAY_MILLIS = 200;
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

    void clear()
    {
        DEBUG::msg("DISPLAY CLEAR");
        digitalWrite(CONST::PIN_LED_1, LOW);
        digitalWrite(CONST::PIN_LED_2, LOW);
        digitalWrite(CONST::PIN_LED_4, LOW);
        digitalWrite(CONST::PIN_LED_8, LOW);
    }

    void setState(LED_STATE s0, LED_STATE s1, LED_STATE s2, LED_STATE s3)
    {
        l0 = s0;
        l1 = s1;
        l2 = s2;
        l3 = s3;
    }

    void setRaw(bool l3, bool l2, bool l1, bool l0)
    {
        digitalWrite(CONST::PIN_LED_1, l0);
        digitalWrite(CONST::PIN_LED_2, l1);
        digitalWrite(CONST::PIN_LED_4, l2);
        digitalWrite(CONST::PIN_LED_8, l3);
    }

    void displayNumberBinary(int number)
    {
        digitalWrite(CONST::PIN_LED_1, IS_BIT_SET(number, 0));
        digitalWrite(CONST::PIN_LED_2, IS_BIT_SET(number, 1));
        digitalWrite(CONST::PIN_LED_4, IS_BIT_SET(number, 2));
        digitalWrite(CONST::PIN_LED_8, IS_BIT_SET(number, 3));
    }

    void displayNumberBlinking(int number)
    {
        DEBUG::log("NUMBER", number);

        int num3 = (number / 1000) % 10;
        int num2 = (number / 100) % 10;
        int num1 = (number / 10) % 10;
        int num0 = number % 10;

        while (num0 > 0 || num1 > 0 || num2 > 0 || num3 > 0)
        {
            EXIT_FUNCTION_IF_NO_KEY();
            digitalWrite(CONST::PIN_LED_1, num0 > 0 ? HIGH : LOW);
            digitalWrite(CONST::PIN_LED_2, num1 > 0 ? HIGH : LOW);
            digitalWrite(CONST::PIN_LED_4, num2 > 0 ? HIGH : LOW);
            digitalWrite(CONST::PIN_LED_8, num3 > 0 ? HIGH : LOW);

            delay(300);
            clear();
            delay(300);

            num0--;
            num1--;
            num2--;
            num3--;
        }
        clear();
    }

    void displayInvalidate()
    {
        if (l0 != l0_prev)
        {
            l0_prev = l0;
        
            switch (l0) {
                case BLINK_ON:
                case ON:
                    l0_val = HIGH;
                    break;
                case BLINK_OFF:
                case OFF:
                    l0_val = LOW;
                    break;
            }

            setRaw(l3_val, l2_val, l1_val, l0_val);
        }

        if (l1 != l1_prev)
        {
            l1_prev = l1;
        
            switch (l1) {
                case BLINK_ON:
                case ON:
                    l1_val = HIGH;
                    break;
                case BLINK_OFF:
                case OFF:
                    l1_val = LOW;
                    break;
            }

            setRaw(l3_val, l2_val, l1_val, l0_val);
        }

        if (l2 != l2_prev)
        {
            l2_prev = l2;
        
            switch (l2) {
                case BLINK_ON:
                case ON:
                    l2_val = HIGH;
                    break;
                case BLINK_OFF:
                case OFF:
                    l2_val = LOW;
                    break;
            }

            setRaw(l3_val, l2_val, l1_val, l0_val);
        }

        if (l3 != l3_prev)
        {
            l3_prev = l3;
        
            switch (l3) {
                case BLINK_ON:
                case ON:
                    l3_val = HIGH;
                    break;
                case BLINK_OFF:
                case OFF:
                    l3_val = LOW;
                    break;
            }

            setRaw(l3_val, l2_val, l1_val, l0_val);
        }
        
        // switching blinks
        if (
            l0 == LED_STATE::BLINK_OFF || l0 == LED_STATE::BLINK_ON
                || 
            l1 == LED_STATE::BLINK_OFF || l1 == LED_STATE::BLINK_ON
                ||
            l2 == LED_STATE::BLINK_OFF || l2 == LED_STATE::BLINK_ON
                ||
            l3 == LED_STATE::BLINK_OFF || l3 == LED_STATE::BLINK_ON
        ) {
            unsigned long currentMillis = millis();
            if ((currentMillis - ledLastBlinkMillis) >= LED_BLINK_DELAY_MILLIS)
            {
                ledLastBlinkMillis = currentMillis;
                //invert blinks!

                if (l0 == LED_STATE::BLINK_ON || l0 == LED_STATE::BLINK_OFF)
                {
                    l0_val = !l0_val;
                }
                if (l1 == LED_STATE::BLINK_ON || l1 == LED_STATE::BLINK_OFF)
                {
                    l1_val = !l1_val;
                }
                if (l2 == LED_STATE::BLINK_ON || l2 == LED_STATE::BLINK_OFF)
                {
                    l2_val = !l2_val;
                }
                if (l3 == LED_STATE::BLINK_ON || l3 == LED_STATE::BLINK_OFF)
                {
                    l3_val = !l3_val;
                }

                setRaw(l3_val, l2_val, l1_val, l0_val);
            }
        }
    }

    void progress(int percentage)
    {
        DEBUG::log("PROGRESS", percentage);
    
        LED_STATE s0 = LED_STATE::OFF;
        LED_STATE s1 = LED_STATE::OFF;
        LED_STATE s2 = LED_STATE::OFF;
        LED_STATE s3 = LED_STATE::OFF;

        if (percentage >= 0 && percentage < 25) {
            s0 = LED_STATE::BLINK_ON;
        } else if (percentage >= 25 && percentage < 50) {
            s0 = LED_STATE::ON;
            s1 = LED_STATE::BLINK_ON;
        } else if (percentage >= 50 && percentage < 75) {
            s0 = LED_STATE::ON;
            s1 = LED_STATE::ON;
            s2 = LED_STATE::BLINK_ON;
        } else if (percentage >= 75 && percentage < 100) {
            s0 = LED_STATE::ON;
            s1 = LED_STATE::ON;
            s2 = LED_STATE::ON;
            s3 = LED_STATE::BLINK_ON;
        } else if (percentage >= 100) {
            s0 = LED_STATE::ON;
            s1 = LED_STATE::ON;
            s2 = LED_STATE::ON;
            s3 = LED_STATE::ON;
        }

        setState(s0, s1, s2, s3);
    }
}