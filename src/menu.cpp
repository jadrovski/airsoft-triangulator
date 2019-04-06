#include "led_display.h"
#include "Arduino.h"
#include "led_display.h"

namespace MENU
{
    int currentMenuItemIndex = 0;

    struct menuItem
    {
        int values[8];
    };

    struct menuLedPair
    {
        LED::LED_STATE left;
        LED::LED_STATE right;
    };

    menuItem items[] = {
        {{1, 2, 3}},
        {{100, 200}}
    };

    menuLedPair getPairByIndex(int idx)
    {
        switch (idx) {
            case 0:
                return {LED::LED_STATE::OFF, LED::LED_STATE::OFF};
            case 1:
                return {LED::LED_STATE::OFF, LED::LED_STATE::BLINK_ON};
            case 2:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::BLINK_ON};
            case 3:
                return {LED::LED_STATE::OFF, LED::LED_STATE::ON};
            case 4:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::ON};
            case 5:
                return {LED::LED_STATE::ON, LED::LED_STATE::ON};
            case 6:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::OFF};
            case 7:
                return {LED::LED_STATE::ON, LED::LED_STATE::OFF};
            default:
                return {LED::LED_STATE::OFF, LED::LED_STATE::OFF};
        }
    }

    void setMenuItem(int idx1, int idx2)
    {
        menuLedPair p1 = getPairByIndex(idx1);
        menuLedPair p2 = getPairByIndex(idx2);
        LED::setState(p2.right, p2.left, p1.right, p1.left);
    }
}