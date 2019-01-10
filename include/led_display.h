#ifndef LED_H_
#define LED_H_

namespace LED {
    void displayState(bool l3, bool l2, bool l1, bool l0);
    void displayNumber(int number);
    void displayNumberEncoded(int number);
    void clear();
}

#endif