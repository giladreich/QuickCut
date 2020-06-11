
#pragma once

#include "QuickCutConsole.h"

#include <Windows.h>


class QuickCutConsoleWindows : public QuickCutConsole
{

public:
    QuickCutConsoleWindows(int argc, char * argv[]);
    ~QuickCutConsoleWindows();

    bool start() override;
    bool stop() override;

    static LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static bool isKeyDown(byte byKey);
    static void printKeyName(KBDLLHOOKSTRUCT * pKbd);

public:
    static HHOOK s_hHook;

};
