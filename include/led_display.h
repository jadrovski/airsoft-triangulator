#ifndef LED_H_
#define LED_H_

namespace LED {
    
    typedef enum
    {
        OFF,
        BLINK_OFF,
        BLINK_ON,
        ON
    } LED_STATE;

    void clear();
    void displayInvalidate();
    void progress(int percentage);
    void setState(LED_STATE s0, LED_STATE s1, LED_STATE s2, LED_STATE s3);
    void displayNumberBinary(int number);
    void displayNumberBlinking(int number);
}

#endif
