#ifndef DEBUG_H_
#define DEBUG_H_

namespace DEBUG {
    void log(const __FlashStringHelper *param, int value);
    void msg(const __FlashStringHelper *msg);
}

#endif
