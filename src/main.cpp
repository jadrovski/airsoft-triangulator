#include <Arduino.h>
#include <LowPower.h>
#include "pin.h"
#include "led_display.h"
#include "buzzer.h"
#include "timeline.h"
#include "key.h"
#include "menu.h"

#define EXIT_FUNCTION_IF_NO_KEY() \
    if (!KEY::isConnected)        \
    return

int btn1State;
int btn2State;
int lastBtn1State;
int lastBtn2State;
unsigned long lastDebounceTimeBtn1 = 0;
unsigned long lastDebounceTimeBtn2 = 0;

unsigned int tweetTimerMillis  = 0;
unsigned int disarmTimerMillis = 0;
bool disarmed    = false;
bool serviceMode = true;

void readBtns()
{
    int btn1 = digitalRead(PIN::BTN_1);
    int btn2 = digitalRead(PIN::BTN_2);

    if (btn1 != lastBtn1State)
    {
        lastDebounceTimeBtn1 = millis();
    }
    if (btn2 != lastBtn2State)
    {
        lastDebounceTimeBtn2 = millis();
    }

    if ((millis() - lastDebounceTimeBtn1) > TIMELINE::DEBOUNCE_MILLIS)
    {
        if (btn1 != btn1State)
        {
            btn1State = btn1;
        }
    }
    if ((millis() - lastDebounceTimeBtn2) > TIMELINE::DEBOUNCE_MILLIS)
    {
        if (btn2 != btn2State)
        {
            btn2State = btn2;
        }
    }

    lastBtn1State = btn1;
    lastBtn2State = btn2;
}

void serviceLoop()
{
    MENU::setMenuItem(2, 6);
    while (1)
    {
        MENU::displayInvalidate();
    }
}

void disarmLoop()
{
    LED::displayNumber(15);
    delay(10);
    LED::clear();
    delay(1000);

    readBtns();

    while (btn1State && btn2State && disarmTimerMillis <= TIMELINE::DISARM_MILLIS)
    {
        unsigned long lastMillis = millis();
        delay(10);
        LED::displayNumber(random(0, 15));
        disarmTimerMillis += (millis() - lastMillis);
        readBtns();

        EXIT_FUNCTION_IF_NO_KEY();
    }
    LED::clear();
    if (disarmTimerMillis > TIMELINE::DISARM_MILLIS)
    {
        disarmed = true;
        LED::displayNumber(15);
    }
}

void welcome()
{
    for (int i = 0; i < 10; i++)
    {
        EXIT_FUNCTION_IF_NO_KEY();

        LED::displayNumber(15);
        BUZZER::tweet(20);
        LED::clear();
        delay(20);
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(PIN::KEY_WAKE_UP, INPUT);

    pinMode(PIN::LED_1, OUTPUT);
    pinMode(PIN::LED_2, OUTPUT);
    pinMode(PIN::LED_4, OUTPUT);
    pinMode(PIN::LED_8, OUTPUT);

    pinMode(PIN::BTN_1, INPUT);
    pinMode(PIN::BTN_2, INPUT);

    attachInterrupt(0, KEY::updateConnectedFlag, CHANGE);
}

void loop()
{
    KEY::updateConnectedFlag();

    if (KEY::isConnected)
    {
        delay(500);
        welcome();

        while (KEY::isConnected)
        {
            if (serviceMode)
            {
                serviceLoop();
            }
            if (!disarmed)
            {
                disarmTimerMillis = 0;
                disarmLoop();
            }
            else
            {
            }
        }
    }
    else if (tweetTimerMillis >= TIMELINE::TWEET_DELAY_MILLIS)
    {
        BUZZER::tweet(200);
        tweetTimerMillis = 0;
    }

    // going to sleep
    LED::clear();
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

    // awaken
    tweetTimerMillis += 1000;
}