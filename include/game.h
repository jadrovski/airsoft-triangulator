#ifndef GAME_H_
#define GAME_H_

#define EXIT_FUNCTION_IF_NO_KEY() \
    if (!GAME::isKeyConnected())  \
    return

namespace GAME {
    bool isKeyConnected();
    void updateKeyConnectedFlag();
    int  getConnectedKeyIdx();
    bool isSuper();
    int  getRadius();
}

#endif
