
#pragma once

#include "pch.h"
#include "QuickCutConsole.h"

class QuickCutConsoleWindows : public QuickCutConsole
{

public:
    QuickCutConsoleWindows(int argc, char * argv[]);
    ~QuickCutConsoleWindows();

    bool start() override;
    bool stop() override;

    static LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static bool             isKeyDown(BYTE key);
    static void             printKeyName(KBDLLHOOKSTRUCT * kbd);

public:
    static HHOOK s_Hook;
};
