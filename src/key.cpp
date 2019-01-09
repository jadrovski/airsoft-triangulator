#include <Arduino.h>
#include "pin.h"

namespace KEY {

    #define TEAMS_COUNT 2

    int teams[] = {130, 205};

    volatile bool isConnected = false;
    
    void updateConnectedFlag()
    {
        isConnected = digitalRead(PIN::KEY_WAKE_UP);
    }

    int getConnectedKeyId()
    {
        //Serial.print("millis ");
        //Serial.print(millis());
        //Serial.print("\n");

        int adcRaw = analogRead(PIN::KEY_ADC);

        if (adcRaw)
        {
            //Serial.print("ADC ");
            //Serial.println(adcRaw);

            for (int i = 0; i < TEAMS_COUNT; i++)
            {
                if (abs(adcRaw - teams[i]) < 10)
                {
                    return i;
                }
            }
        }

        return -1;
    }
}