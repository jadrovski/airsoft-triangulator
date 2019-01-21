#include <Arduino.h>
#include <EEPROM.h>

namespace MEM {
    //This function will write a 4 byte (32bit) long to the eeprom at
    //the specified address to address + 3.
    void writeLong(int address, long value)
    {
        //Decomposition from a long to 4 bytes by using bitshift.
        //One = Most significant -> Four = Least significant byte
        byte four = (value & 0xFF);
        byte three = ((value >> 8) & 0xFF);
        byte two = ((value >> 16) & 0xFF);
        byte one = ((value >> 24) & 0xFF);

        //Write the 4 bytes into the eeprom memory.
        EEPROM.update(address, four);
        EEPROM.update(address + 1, three);
        EEPROM.update(address + 2, two);
        EEPROM.update(address + 3, one);
    }

    //This function will return a 4 byte (32bit) long from the eeprom
    //at the specified address to address + 3.
    long readLong(int address)
    {
        //Read the 4 bytes from the eeprom memory.
        long four = EEPROM.read(address);
        long three = EEPROM.read(address + 1);
        long two = EEPROM.read(address + 2);
        long one = EEPROM.read(address + 3);

        //Return the recomposed long by using bitshift.
        return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
    }
}
