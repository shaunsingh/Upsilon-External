/*
 *--------------------------------------
 * Program Name: hp41c
 * Author: shaunsingh
 * License: free
 * Description: scanner key emulator
 *--------------------------------------
*/

#include "../../../api/extapp_api.h"

#include "../emu/nut.h"
#include "../lcd/lcd.h"

#define K_WAIT 0
#define K_IDLE 1
#define K_PRESSED 2
#define K_RELEASED 3
#define K_WAIT_CHECK 4

static uint8_t keyDown = 0;
static uint8_t keyCounter = 0;

uint8_t keyState = K_WAIT;					
static uint8_t mscOn = 0;

void kbdPressKey(uint8_t key) {
    if ((nutSleep && !lcdEnable && (key == 0x18)) || (!nutSleep || lcdEnable)) {
        nutKey = key;
        keyDown = 1;
        if (keyState == K_IDLE) {
            keyState = K_PRESSED;
        }
    }
}

void kbdReleaseKey() {
    keyDown = 0;
    if (keyState == K_PRESSED) {
        keyState = K_RELEASED;
    }
} 

uint8_t scannerOpCheckKeyboard() {
    uint8_t kyf = (keyState == K_PRESSED) || (keyState == K_RELEASED);
    if (keyState == K_WAIT_CHECK) {
        keyState = K_WAIT;
        keyCounter = 16;
    }
    if (mscOn) {
        keyCounter--;
        if (keyCounter == 0) {
            mscOn = 0;
            keyDown = 1;
            keyState = K_PRESSED; 
        }
    }
    return (kyf) ? 1 : 0;
}

void scannerOpResetKeyboard() {
    if (keyState == K_RELEASED) {
        keyState = K_WAIT_CHECK;
    }
}

void scannerLoop() {
    if (nutSleep) {
        if (keyState == K_RELEASED)
            keyState = K_IDLE;
        if ((keyState == K_PRESSED)) { 
            nutWake();
        }
        if ((keyState == K_WAIT)) { 
            if (keyDown) {
                keyState = K_PRESSED;
                nutWake();
            } else 
                keyState = K_IDLE;
        } 
    } else {
        if (keyState == K_WAIT) {
            if (keyCounter == 0) {
                if (keyDown)
                    keyState = K_PRESSED;
                else
                    keyState = K_IDLE;
            } else
                keyCounter--;
        }
    }
}
