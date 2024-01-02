#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConsoleApplication1.h"
#include <chrono>

using namespace std;

int* GetPixel(int x, int y) {
    HDC hdcScreen = ::GetDC(NULL);
    COLORREF pixel = ::GetPixel(hdcScreen, x, y);
    ReleaseDC(NULL, hdcScreen);
    int red = GetRValue(pixel);
    int green = GetGValue(pixel);
    int blue = GetBValue(pixel);
    int pixelColors[3] = { red, green, blue };
    return pixelColors;
}

int midpointX = 960;
int midpointY = 540;
int black = 20; //If a colour is less than or equal to this
int shieldR1 = 35;
int shieldR2 = 65;
int shieldG1 = 50;
int shieldG2 = 70;
typedef std::chrono::high_resolution_clock hiresclock;

bool IfSpear(int colour[3]) {
    bool isBlack = colour[0] <= black && colour[1] <= black; //If the colour is the background(black). B is not included as sometimes black is(0, 0, 70)
    bool isWhite = colour[0] == colour[1] && colour[1] == colour[2]; //All 3 colours being the same is only true for the border of the box and for the characters
    bool isShield = colour[0] >= shieldR1 && colour[0] <= shieldR2 && colour[1] >= shieldG1 && colour[1] <= shieldG2; //If the colour is the blue shield
    return !(isBlack || isWhite || isShield);
    //IE return  if not (If the colour is the background(black), if and ''''''
}

void PressKey(int key) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = key;///key; // 0x41 is the virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
}

int main()
{
    while (true) {
        static auto timer = hiresclock::now();
        auto milisec = (hiresclock::now() - timer).count() / 1000000;			
        if (milisec > 0.1f * 1000)
        {
            //Manages timer
            timer = hiresclock::now();
            if (!(GetKeyState('Q') & 0x8000)) {
                for (int i = 100; i < 141; i += 40) { //100 and 140
                    if (IfSpear(GetPixel(midpointX - i, midpointY))) {
                        PressKey(0x25);
                        //printf("Left");
                        break;
                    }
                    else if (IfSpear(GetPixel(midpointX + i, midpointY))) {
                        PressKey(0x27);
                        //printf("Right");
                        break;
                    }
                    else if (IfSpear(GetPixel(midpointX, midpointY - i))) {
                        PressKey(0x26);
                        //printf("Up");
                        break;
                    }
                    else if (IfSpear(GetPixel(midpointX, midpointY + i))) {
                        PressKey(0x28);
                        //printf("Down");
                        break;
                    }
                }
            }
        }
    }
}