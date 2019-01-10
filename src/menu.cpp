#include "led_display.h"
#include "Arduino.h"

namespace MENU
{

enum led_states
{
    OFF,
    BLINK_OFF,
    BLINK_ON,
    ON
};

int currentMenuItemIndex = 0;

led_states l3 = led_states::OFF;
led_states l2 = led_states::OFF;
led_states l1 = led_states::OFF;
led_states l0 = led_states::OFF;

led_states l3_prev = led_states::OFF;
led_states l2_prev = led_states::OFF;
led_states l1_prev = led_states::OFF;
led_states l0_prev = led_states::OFF;

int l3_val = LOW;
int l2_val = LOW;
int l1_val = LOW;
int l0_val = LOW;

struct menu_item
{
    int values[8];
};

struct menu_led_pair
{
    led_states left;
    led_states right;
};

menu_item items[] = {
    {{1, 2, 3}},
    {{100, 200}}
};

const unsigned int LED_BLINK_DELAY_MILLIS = 200;
unsigned long ledLastBlinkMillis = 0;

menu_led_pair getPairByIndex(int idx)
{
    switch (idx) {
        case 0:
            return {led_states::OFF, led_states::OFF};
        case 1:
            return {led_states::OFF, led_states::BLINK_ON};
        case 2:
            return {led_states::BLINK_ON, led_states::BLINK_ON};
        case 3:
            return {led_states::OFF, led_states::ON};
        case 4:
            return {led_states::BLINK_ON, led_states::ON};
        case 5:
            return {led_states::ON, led_states::ON};
        case 6:
            return {led_states::BLINK_ON, led_states::OFF};
        case 7:
            return {led_states::ON, led_states::OFF};
    }
}

void setMenuItem(int idx1, int idx2)
{
    menu_led_pair p1 = getPairByIndex(idx1);
    menu_led_pair p2 = getPairByIndex(idx2);
    l3 = p1.left;
    l2 = p1.right;
    l1 = p2.left;
    l0 = p2.right;
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

        LED::displayState(l3_val, l2_val, l1_val, l0_val);
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

        LED::displayState(l3_val, l2_val, l1_val, l0_val);
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

        LED::displayState(l3_val, l2_val, l1_val, l0_val);
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

        LED::displayState(l3_val, l2_val, l1_val, l0_val);
    }
     
    // switching blinks
    if (
        l0 == led_states::BLINK_OFF || l0 == led_states::BLINK_ON
            || 
        l1 == led_states::BLINK_OFF || l1 == led_states::BLINK_ON
            ||
        l2 == led_states::BLINK_OFF || l2 == led_states::BLINK_ON
            ||
        l3 == led_states::BLINK_OFF || l3 == led_states::BLINK_ON
    ) {
        unsigned long currentMillis = millis();
        if ((currentMillis - ledLastBlinkMillis) >= LED_BLINK_DELAY_MILLIS)
        {
            ledLastBlinkMillis = currentMillis;
            //invert blinks!

            if (l0 == led_states::BLINK_ON || l0 == led_states::BLINK_OFF)
            {
                l0_val = !l0_val;
            }
            if (l1 == led_states::BLINK_ON || l1 == led_states::BLINK_OFF)
            {
                l1_val = !l1_val;
            }
            if (l2 == led_states::BLINK_ON || l2 == led_states::BLINK_OFF)
            {
                l2_val = !l2_val;
            }
            if (l3 == led_states::BLINK_ON || l3 == led_states::BLINK_OFF)
            {
                l3_val = !l3_val;
            }

            LED::displayState(l3_val, l2_val, l1_val, l0_val);
        }
    }
}

}