
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsoleUnix.h"

#include "QuickCutShared/Managers/ProfileManager.h"

QuickCutConsoleUnix::QuickCutConsoleUnix(int argc, char * argv[])
    : QuickCutConsole(argc, argv)
{
    s_Instance = this;
}

QuickCutConsoleUnix::~QuickCutConsoleUnix() {}

bool QuickCutConsoleUnix::start()
{
    if (!QuickCutConsole::start())
    {
        qDebug() << "[QuickCutConsoleUnix::start]: Failed to start...";
        return false;
    }

    return true;
}

bool QuickCutConsoleUnix::stop()
{
    if (!QuickCutConsole::stop())
    {
        qDebug() << "[QuickCutConsoleUnix::stop]: Failed to stop...";
        return false;
    }

    return true;
}