#include <Arduino.h>
#include <Control.h>
#include <LedDisplay.h>
#include <Buzzer.h>
#include <TriangulatorGame.h>
#include <Key.h>
#include <Debug.h>
#include "const.h"

#define EXIT_IF_NO_KEY() if (!key.isConnected()) return

bool isKeyDisconnected();

Debug debug(CONST::DEBUG);
Buzzer buzzer(CONST::PIN_BUZZER, debug);
Control control(CONST::PIN_BTN_LEFT, CONST::PIN_BTN_RIGHT, Control::TIMELINE_CONTROL_DEBOUNCE_MILLIS, debug);
Key key(
    CONST::PIN_KEY_WAKE_UP,
    CONST::PIN_KEY_ADC,
    CONST::KEY_TEAMS_COUNT,
    CONST::KEY_TEAM_RAW_VALUES,
    CONST::KEY_SUPER_IDX,
    debug
);
LedDisplay ledDisplay(
    CONST::PIN_LED_1,
    CONST::PIN_LED_2,
    CONST::PIN_LED_4,
    CONST::PIN_LED_8,
    LedDisplay::LED_BLINK_FAST_DELAY_MILLIS,
    LedDisplay::LED_BLINK_SLOW_DELAY_MILLIS,
    &debug,
    isKeyDisconnected
);
TriangulatorGame triangulatorGame(
        CONST::GAME_DEVICE_ID,
        CONST::GAME_RADIUSES,
        buzzer,
        control,
        key,
        ledDisplay,
        debug
);

bool isKeyDisconnected()
{
    return !key.isConnected();
}

void updateKey()
{
    key.updateConnectedFlag();
}

void setup()
{
    debug.log(F("VER"), CONST::VERSION);

    Serial.begin(115200);

    ledDisplay.initHardware();
    buzzer.initHardware();
    control.initHardware();
    key.initHardware();

    attachInterrupt(0, &updateKey, CHANGE);

    triangulatorGame.init();
}

void loop()
{
    triangulatorGame.run();
}