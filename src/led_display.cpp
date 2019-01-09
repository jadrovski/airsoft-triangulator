#include <Arduino.h>
#include "pin.h"
#include "util.h"

namespace LED {
    void displayNumber(int number)
    {
        digitalWrite(PIN::LED_1, IS_BIT_SET(number, 0));
        digitalWrite(PIN::LED_2, IS_BIT_SET(number, 1));
        digitalWrite(PIN::LED_4, IS_BIT_SET(number, 2));
        digitalWrite(PIN::LED_8, IS_BIT_SET(number, 3));
    }

    void displayNumberEncoded(int number)
    {
        int num3 = (number / 1000) % 10;
        int num2 = (number / 100) % 10;
        int num1 = (number / 10) % 10;
        int num0 = number % 10;

        while (num0 > 0 || num1 > 0 || num2 > 0 || num3 > 0)
        {
            digitalWrite(PIN::LED_1, num0 > 0 ? HIGH : LOW);
            digitalWrite(PIN::LED_2, num1 > 0 ? HIGH : LOW);
            digitalWrite(PIN::LED_4, num2 > 0 ? HIGH : LOW);
            digitalWrite(PIN::LED_8, num3 > 0 ? HIGH : LOW);

            delay(300);
            displayNumber(0);
            delay(300);

            num0--;
            num1--;
            num2--;
            num3--;
        }
        displayNumber(0);
    }

    void clear()
    {
        digitalWrite(PIN::LED_1, LOW);
        digitalWrite(PIN::LED_2, LOW);
        digitalWrite(PIN::LED_4, LOW);
        digitalWrite(PIN::LED_8, LOW);
    }
}