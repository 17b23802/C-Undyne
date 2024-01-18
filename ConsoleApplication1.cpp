#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "ConsoleApplication1.h"

using namespace std;
HDC hdcScreen = ::GetDC(NULL);

int* GetPixel(int x, int y) {
    
    COLORREF pixel = ::GetPixel(hdcScreen, x, y);
    //ReleaseDC(NULL, hdcScreen);
    int red = GetRValue(pixel);
    int green = GetGValue(pixel);
    //int blue = GetBValue(pixel);
    //int pixelColors[3] = { red, green, blue };
    int pixelColors[2] = { red, green };
    return pixelColors;
}

int midpointX = 960;
int midpointY = 540;
int black = 70; //If a colour is less than or equal to this
int shieldR1 = 35;
int shieldR2 = 65;
int shieldG1 = 50;
int shieldG2 = 70;
int lastDirection = 1; //1=Left, 2=Right, 3=Down, 4=Up
int innerI = 83;

bool IsSpear(int colour[2]) {
    return colour[0] > black || colour[1] > black; //If the colour is the background(black). B is not included as sometimes black is(0, 0, 70)
    //bool isWhite = colour[0] == colour[1] && colour[1] == colour[2]; //All 3 colours being the same is only true for the border of the box and for the characters
    //bool isShield = colour[0] >= shieldR1 && colour[0] <= shieldR2 && colour[1] >= shieldG1 && colour[1] <= shieldG2; //If the colour is the blue shield
    //return !(isBlack || isWhite || isShield);
}

void PressKey(char key) {
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = key;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = key;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    //INPUT ip;
    //ip.type = INPUT_KEYBOARD;
    //ip.ki.time = 0; //Cause 1/30=33 so 1 frame per 33 ms

    //ip.ki.wVk = key;///key; // 0x41 is the virtual-key code for the "a" key
    //ip.ki.dwFlags = 0; // 0 for key press
    //SendInput(1, &ip, sizeof(INPUT));
}


int main()
{
    while (true) {
        if (lastDirection != 1) {
            if (IsSpear(GetPixel(midpointX - innerI, midpointY))) {
                //PressKey(0x25); //Left
                PressKey('A'); //A
                //printf("Left");
                lastDirection = 1;
                continue;
            }
        }
        if (lastDirection != 2) {
            if (IsSpear(GetPixel(midpointX + innerI, midpointY))) {
                //PressKey(0x27); //Right
                PressKey('D'); //D
                //printf("Right");
                lastDirection = 2;
                continue;
            }
        }
        if (lastDirection != 3) {
            if (IsSpear(GetPixel(midpointX, midpointY - innerI))) {
                PressKey(0x26); //Up
                PressKey('W'); //W
                //printf("Up");
                lastDirection = 3;
                continue;
            }
        }
        if (lastDirection != 4) {
            if (IsSpear(GetPixel(midpointX, midpointY + innerI))) {
                //PressKey(0x28); //Down
                PressKey('S'); //S
                //printf("Down");
                lastDirection = 4;
                continue;
            }
        }
    }
}