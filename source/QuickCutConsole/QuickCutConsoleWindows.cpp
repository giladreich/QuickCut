
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

    // TODO(Gilad): Re-work models to store key-codes + key-names. It will make it easier
    // also using that data in the UI.
}

void QuickCutConsoleWindows::executeProcess(const QString & process, const QString & arguments)
{
    // Bypasses OS automatic redirection on 32-bit x86 builds. i.e. SnippingTool.
    // https://docs.microsoft.com/en-us/windows/win32/winprog64/file-system-redirector
    QString target = process;
#if defined(Q_PROCESSOR_X86_32)
    QString winDir = QDir::fromNativeSeparators(QString(qgetenv("windir")).toLower());
    if (winDir.isEmpty())
        winDir = QDir::fromNativeSeparators(QString(qgetenv("SystemRoot")).toLower());
    QString sys32Dir     = QString("%1/system32").arg(winDir);
    QString sysNativeDir = QString("%1/sysnative").arg(winDir);

    TCHAR buff[_MAX_PATH]{};
    ExpandEnvironmentStrings(qUtf16Printable(target), reinterpret_cast<LPWSTR>(&buff),
                             _MAX_PATH);

    target = QDir::fromNativeSeparators(QString::fromWCharArray(buff).toLower());
    if (target.contains(sys32Dir))
    {
        target = target.replace(sys32Dir, sysNativeDir);
        // target = QDir::toNativeSeparators(target);
    }
#endif

    QString command = QString(R"(cmd /c start "" "%1")").arg(target);

    QStringList argsTmp = arguments.trimmed().split(',');
    for (auto && arg : argsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        command += ' ' + argTrimmed;
    }
    qDebug() << "[QuickCutConsoleWindows::executeProcess] - Command: " << qPrintable(command);

    WinExec(qPrintable(command), SW_SHOW);
}
