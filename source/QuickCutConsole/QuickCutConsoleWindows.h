
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

public:
};
