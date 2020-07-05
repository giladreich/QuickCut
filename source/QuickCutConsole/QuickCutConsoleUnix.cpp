
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

void QuickCutConsoleUnix::sendInput(const QStringList & dstKeys)
{
    if (dstKeys.isEmpty()) return;

    // TODO(Gilad): Send inputs on unix systems.
}

void QuickCutConsoleUnix::executeProcess(const QString & process, const QString & arguments)
{
    QString command = QString(R"(sh -c '%1')").arg(process);

    QStringList argsTmp = arguments.trimmed().split(",");
    for (auto && arg : argsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        command += " " + argTrimmed;
    }
    qDebug() << "[QuickCutConsoleUnix::executeProcess] - Command: " << qPrintable(command);

    QProcess::execute(command, QStringList());
}
