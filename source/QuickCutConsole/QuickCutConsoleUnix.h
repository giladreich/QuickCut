
#pragma once

#include "QuickCutConsole.h"


class QuickCutConsoleUnix : public QuickCutConsole
{

public:
    QuickCutConsoleUnix(int argc, char * argv[]);
    ~QuickCutConsoleUnix();

    bool start() override;
    bool stop() override;

public:

};
