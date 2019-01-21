#ifndef LED_H_
#define LED_H_

namespace LED {
    void clear();
    void displayState(bool l3, bool l2, bool l1, bool l0);
    void displayNumberBinary(int number);
    void displayNumberBlinking(int number);
}

#endif
