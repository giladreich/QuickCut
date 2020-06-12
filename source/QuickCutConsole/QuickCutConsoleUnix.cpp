

#include "QuickCutConsoleUnix.h"
#include "pch.h"
#include "Profile.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>

QuickCutConsoleUnix::QuickCutConsoleUnix(int argc, char * argv[])
    : QuickCutConsole(argc, argv)
{
    s_pInstance = this;
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
