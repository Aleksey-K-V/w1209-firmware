/**
 * Implementation of application menu.
 */

#include "ts.h"

#define MENU_1_SEC_PASSED   17
#define MENU_3_SEC_PASSED   50
#define MENU_5_SEC_PASSED   90
 
static unsigned char menuDisplay;
static unsigned char menuState;
static unsigned int timer;
 
void initMenu() {
    timer = 0;
    menuState = menuDisplay = MENU_ROOT;
}

unsigned char getMenuDisplay() {
    return menuDisplay;
}

/**
 * Updating state of application's menu when new event is received.
 */
void feedMenu(unsigned char event) {
    if (menuState == MENU_ROOT) {
        switch(event) {
            case MENU_EVENT_PUSH_BUTTON1:
                timer = 0;
                menuDisplay = MENU_SET_THRESHOLD;
                break;
            case MENU_EVENT_RELEASE_BUTTON1:
                if (timer < MENU_5_SEC_PASSED) {
                    menuState = MENU_SET_THRESHOLD;
                }
                timer = 0;
                break;
            case MENU_EVENT_CHECK_TIMER:
                if (getButton1()) {
                    timer++;
                    if(timer > MENU_5_SEC_PASSED) {
                        setParamId(0);
                        timer = 0;
                        menuState = menuDisplay = MENU_SELECT_PARAM;
                    }
                }
                break;
            default:
                if (timer > MENU_5_SEC_PASSED) {
                    timer = 0;
                    menuState = menuDisplay = MENU_ROOT;
                }
                break;
        }
    } else if(menuState == MENU_SELECT_PARAM) {
        // TODO: select param +/-, go to MENU_CHANGE_PARAM,
        // store on timeout and return to MENU_ROOT
        switch(event) {
            case MENU_EVENT_PUSH_BUTTON1:
                timer = 0;
                menuState = menuDisplay = MENU_CHANGE_PARAM;
                break;
            case MENU_EVENT_RELEASE_BUTTON1:
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON2:
//        RELAY_PORT ^= RELAY_BIT; //toggle LED
                incParamId();
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON3:
                decParamId();
                timer = 0;
                break;
            case MENU_EVENT_CHECK_TIMER:
                timer++;
                if (getButton1() && timer > MENU_3_SEC_PASSED) {
                    timer = 0;
                    menuState = menuDisplay = MENU_SELECT_PARAM;
                    break;
                }
                if (timer > MENU_1_SEC_PASSED) {
                    if (getButton2()) {
                        incParamId();
                    } else if (getButton3()) {
                        decParamId();
                    }
                } else if (timer > MENU_5_SEC_PASSED) {
        RELAY_PORT ^= RELAY_BIT; //toggle LED
                    timer = 0;
                    setParamId(0);
                    menuState = menuDisplay = MENU_ROOT;
                }
                break;
            default:
                break;
        }
    } else if(menuState == MENU_CHANGE_PARAM) {
        // TODO: value +/-, return to MENU_SELECT_PARAM,
        // store on timeout and return to MENU_ROOT
        switch(event) {
            case MENU_EVENT_PUSH_BUTTON1:
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON2:
                incParam();
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON3:
                decParam();
                timer = 0;
                break;
            case MENU_EVENT_CHECK_TIMER:
                timer++;
                if (timer > MENU_5_SEC_PASSED) {
                    timer = 0;
                    menuState = menuDisplay = MENU_ROOT;
                }
                break;
            default:
                break;
        }
    } else if(menuState == MENU_SET_THRESHOLD) {
        // TODO: threshold +/-, on button1 store and return to MENU_ROOT,
        // on timeout store and return to MENU_ROOT
        switch(event) {
            case MENU_EVENT_PUSH_BUTTON1:
                timer = 0;
                menuDisplay = MENU_ROOT;
                break;
            case MENU_EVENT_RELEASE_BUTTON1:
                if (timer < MENU_5_SEC_PASSED) {
                    menuState = MENU_ROOT;
                }
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON2:
                setParamId(EEPROM_PARAM_THRESHOLD);
                incParam();
                timer = 0;
                break;
            case MENU_EVENT_PUSH_BUTTON3:
                setParamId(EEPROM_PARAM_THRESHOLD);
                decParam();
                timer = 0;
                break;
            case MENU_EVENT_CHECK_TIMER:
                timer++;
                if (getButton1() && timer > MENU_5_SEC_PASSED) {
                    timer = 0;
                    menuState = menuDisplay = MENU_SELECT_PARAM;
                }
                if (timer > MENU_5_SEC_PASSED) {
                    timer = 0;
                    menuState = menuDisplay = MENU_ROOT;
                }
                break;
            default:
                break;
        }
    }
}
 
void refreshMenu() {
    feedMenu(MENU_EVENT_CHECK_TIMER);
}