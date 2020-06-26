
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsoleWindows.h"

#include "QuickCutShared/Managers/ProfileManager.h"

QuickCutConsoleWindows::QuickCutConsoleWindows(int argc, char * argv[])
    : QuickCutConsole(argc, argv)
{
    s_Instance = this;
}

QuickCutConsoleWindows::~QuickCutConsoleWindows() {}

bool QuickCutConsoleWindows::start()
{
    if (!QuickCutConsole::start())
    {
        qDebug() << "[QuickCutConsoleWindows::start]: Failed to start...";
        return false;
    }

    return true;
}

bool QuickCutConsoleWindows::stop()
{
    if (!QuickCutConsole::stop())
    {
        qDebug() << "[QuickCutConsoleWindows::stop]: Failed to stop...";
        return false;
    }

    return true;
}
