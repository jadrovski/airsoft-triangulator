#ifndef NEPOPAL_BUZZER_H_
#define NEPOPAL_BUZZER_H_

#include <Arduino.h>
#include <Debug.h>
#include <AbstractHardware.h>
//#include <MelodyUtils/MelodyUtils.h>
//#include <MelodyUtils/pitches.h>

class Buzzer : public AbstractHardware {
public:
    Buzzer(uint8_t pin, Debug &debug);
    void initHardware() override;
    void tweet(unsigned long ms);
    void notificate();
private:
    uint8_t _pin;
    Debug &_debug;
};

#endif
