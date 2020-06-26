
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

void QuickCutConsoleWindows::sendInput(const QStringList & dstKeys)
{
    if (dstKeys.isEmpty()) return;

    //// TODO(Gilad): Re-work models to store key-codes + key-names. It will make it easier
    ///also / using that data in the UI.
}

void QuickCutConsoleWindows::executeProcess(const QString & process, const QString & arguments)
{
    QString command = QString(R"(cmd /c start "" "%1")").arg(process);

    QStringList argsTmp = arguments.trimmed().split(",");
    for (auto && arg : argsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        command += " " + argTrimmed;
    }
    qDebug() << "[QuickCutConsoleWindows::executeProcess] - Command: " << qPrintable(command);

    WinExec(qPrintable(command), SW_SHOW);
}
