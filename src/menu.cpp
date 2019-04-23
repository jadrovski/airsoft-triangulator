#include "Arduino.h"
#include "led_display.h"
#include "control.h"
#include "debug.h"
#include "buzzer.h"

namespace MENU
{

    struct menuLedPair
    {
        LED::LED_STATE left;
        LED::LED_STATE right;
    };

    menuLedPair getPairByIndex(int idx)
    {
        switch (idx) {
            case 0:
                return {LED::LED_STATE::OFF, LED::LED_STATE::OFF};
            case 1:
                return {LED::LED_STATE::OFF, LED::LED_STATE::BLINK_ON};
            case 2:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::BLINK_ON};
            case 3:
                return {LED::LED_STATE::OFF, LED::LED_STATE::ON};
            case 4:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::ON};
            case 5:
                return {LED::LED_STATE::ON, LED::LED_STATE::ON};
            case 6:
                return {LED::LED_STATE::BLINK_ON, LED::LED_STATE::OFF};
            case 7:
                return {LED::LED_STATE::ON, LED::LED_STATE::OFF};
            default:
                return {LED::LED_STATE::OFF, LED::LED_STATE::OFF};
        }
    }

    void setMenuItem(int idx1, int idx2)
    {
        menuLedPair p1 = getPairByIndex(idx1);
        menuLedPair p2 = getPairByIndex(idx2);
        LED::setState(p2.right, p2.left, p1.right, p1.left);
    }

    int currentMenuItemIndex = 0;

    const unsigned int tMENU_SUB = 0;
    const unsigned int tMENU_EXE = 1;

    const byte KEY_OK = 0;
    const byte KEY_LIST = 1;
    const byte KEY_ESC = 2;

    struct StructMenu
    {
        byte type;
        byte subMenuCount;
        unsigned int val;
        void (*foo) (void);
        StructMenu *subMenu[15];
    };

    void fTest()
    {

    }

    void fAbout()
    {
        
    }

    struct StructMenu Menu1 = {tMENU_EXE,1,0,fAbout};
    struct StructMenu Menu2 = {tMENU_EXE,1,1,fAbout};
    struct StructMenu Menu3 = {tMENU_EXE,1,2,fAbout};
    struct StructMenu Menu4 = {tMENU_EXE,1,3,fAbout};
    struct StructMenu Menu5 = {tMENU_EXE,1,4,fAbout};
    struct StructMenu Menu6 = {tMENU_EXE,1,5,fAbout};
    struct StructMenu Menu7 = {tMENU_EXE,1,6,fAbout};
    struct StructMenu Menu8 = {tMENU_EXE,1,7,fAbout};
    struct StructMenu MenuTest = {tMENU_SUB,8,0,0, {&Menu1,&Menu2,&Menu3,&Menu4,&Menu5,&Menu6,&Menu7,&Menu8}};

    const int MENU_COUNT = 2;

    int CurMenu = 0;

    void DrawMenuHeader(int s)
    {
        menuLedPair p = getPairByIndex(s);
        LED::setS0(p.right);
        LED::setS1(p.left);
    }

    void DrawItem(int s)
    {
        menuLedPair p = getPairByIndex(s);
        DEBUG::log(F("right"), p.right);
        DEBUG::log(F("left"), p.left);
        LED::setS2(p.right);
        LED::setS3(p.left);
    }

    byte KeyGet() 
    {
        CONTROL::updateState();
        if (CONTROL::isBtnLeftPressed()) {
            return KEY_LIST;
        }

        if (CONTROL::isBtnRightPressed()) {
            return KEY_OK;
        }

        return 255;
    }

    void StartMenu()
    {
        struct StructMenu *MenuItem = &MenuTest;
        uint8_t PosCurItemMenu;
        uint8_t CountItemMenu;
        uint8_t Key;
        void (*pointer)();
        uint8_t Type = MenuItem->type;
        
        if (Type == tMENU_SUB) {
            
            CountItemMenu  = MenuItem->subMenuCount;
            Key            = -1;
            PosCurItemMenu = 0;

            DrawMenuHeader(CurMenu);
            DrawItem(PosCurItemMenu);

            LED::displayInvalidate();

            if (Type == tMENU_SUB) {
                PosCurItemMenu = 0;
                do {
                    LED::displayInvalidate();
                    if (Key != KeyGet()) {
                        Key = KeyGet();
                        // Здесь должна быть задержка
                        switch (Key) {
                            case KEY_LIST:
                                if (PosCurItemMenu == CountItemMenu - 1) {
                                    PosCurItemMenu = 0;
                                } else {
                                    PosCurItemMenu ++;
                                }
                                BUZZER::beep();
                                DrawMenuHeader(CurMenu);
                                DrawItem(PosCurItemMenu);
                                break;
                            // case KEY_RIGHT:
                            //     if (MENU_COUNT>0)
                            //     {
                            //         if(CurMenu < MENU_COUNT-1)
                            //         {    
                            //             CurMenu ++;
                            //             MenuItem = &MainMenuItem[CurMenu];
                            //             CountItemMenu = MenuItem->SubMenuCount;
                            //             PosCurItemMenu =0;
                            //             DrawMenuHeader(MenuItem->Caption[Language], CurMenu, MENU_COUNT,0);
                            //             DrawItem(MenuItem->SubMenu[PosCurItemMenu]->Caption[Language],PosCurItemMenu, CountItemMenu,0); 
                            //         }
                            //     }
                            // break;
                            case KEY_OK:
                                switch (MenuItem->subMenu[PosCurItemMenu]->type)
                                {
                                    case tMENU_EXE:
                                        DrawMenuHeader(PosCurItemMenu);
                                        pointer = MenuItem->subMenu[PosCurItemMenu]->foo;   
                                        (*pointer)();
                                        break;
                                    default: break;
                            }
                            break;
                        }
                    }
                } while ((Key != KEY_ESC));
            }
        }
    }
}