
#include "pch.h"
#include "QuickCutConsoleUnix.h"
#include "Managers/ProfileManager.h"

QuickCutConsoleUnix::QuickCutConsoleUnix(int argc, char * argv[])
    : QuickCutConsole(argc, argv)
{
    s_Instance = this;
}

QuickCutConsoleUnix::~QuickCutConsoleUnix() {}

bool QuickCutConsoleUnix::start()
{
    if (!QuickCutConsole::start()) return false;

    // TODO: Implement hook.

    return true;
}

bool QuickCutConsoleUnix::stop()
{
    if (!QuickCutConsole::stop()) return false;

    return true;
}
