#ifndef NEPOPAL_LED_H_
#define NEPOPAL_LED_H_

#include <Arduino.h>
#include <Debug.h>
#include <AbstractHardware.h>

typedef enum
{
    OFF,
    BLINK_OFF,
    BLINK_ON,
    ON
} LED_STATE;

class LedDisplay : public AbstractHardware {
public:
    static const unsigned int LED_BLINK_FAST_DELAY_MILLIS = 200;
    static const unsigned int LED_BLINK_SLOW_DELAY_MILLIS = 400;

    LedDisplay(
        uint8_t pinLed1,
        uint8_t pinLed2,
        uint8_t pinLed4,
        uint8_t pinLed8,
        unsigned int ledBlinkStateDelayMillis,
        unsigned int ledBlinkObviousDelayMillis,
        Debug *debug = nullptr,
        bool (*exitCondition)() = nullptr
    );
    void initHardware() override;
    void clear();
    void displayInvalidate();
    void progress(int percentage);
    void setState(LED_STATE s0, LED_STATE s1, LED_STATE s2, LED_STATE s3);
    void setS0(LED_STATE s);
    void setS1(LED_STATE s);
    void setS2(LED_STATE s);
    void setS3(LED_STATE s);
    void displayNumberBinary(unsigned int number);
    void displayNumberBlinking(unsigned int number);
private:
    bool (*_exitCondition)() = nullptr;
    void setRaw(int l3, int l2, int l1, int l0);
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
    uint8_t _pinLed1;
    uint8_t _pinLed2;
    uint8_t _pinLed4;
    uint8_t _pinLed8;
    unsigned int _ledBlinkStateDelayMillis;
    unsigned int _ledBlinkObviousDelayMillis;
    unsigned long _ledLastBlinkMillis = 0;
    Debug *_debug = nullptr;
};

#endif
