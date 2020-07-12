
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

    void sendInput(const KeyboardKeys & dstKeys, KeyboardHook::KeyEvent keyEvent) override;
    void executeProcess(const QString & process, const QString & arguments) override;
};
