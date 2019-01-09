#ifndef KEY_H_
#define KEY_H_

namespace KEY {
    extern volatile bool isConnected;
    void updateConnectedFlag();
    int getConnectedKeyId();
}

#endif