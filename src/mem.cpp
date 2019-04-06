#include <Arduino.h>
#include <EEPROM.h>

namespace MEM {
    void writeULong(int addr, unsigned long num)
    {
        byte raw[4];
        (unsigned long&)raw = num;
        for(byte i = 0; i < 4; i++) EEPROM.update(addr+i, raw[i]);
    }

    unsigned long readULong(int addr)
    {
        byte raw[4];
        for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr+i);
        unsigned long &num = (unsigned long&)raw;
        return num;
    }   
}