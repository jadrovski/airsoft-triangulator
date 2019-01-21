#include <Arduino.h>
#include "const.h"
#include "util.h"
#include "game.h"

namespace LED {
    void clear()
    {
        digitalWrite(CONST::PIN_LED_1, LOW);
        digitalWrite(CONST::PIN_LED_2, LOW);
        digitalWrite(CONST::PIN_LED_4, LOW);
        digitalWrite(CONST::PIN_LED_8, LOW);
    }

    void displayState(bool l3, bool l2, bool l1, bool l0)
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
}
