
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

void QuickCutConsoleWindows::sendInput(const KeyboardKeys &   keys,
                                       KeyboardHook::KeyEvent keyEvent)
{
    if (keys.isEmpty()) return;

    int     inputsCount = keys.size();
    INPUT * inputs      = new INPUT[inputsCount];
    memset(inputs, 0, inputsCount * sizeof(INPUT));

    for (int i = 0; i < inputsCount; ++i)
    {
        INPUT & input = inputs[i];
        switch (keys[i].getKeyCode())
        {
            case VK_LEFT:
            case VK_UP:
            case VK_RIGHT:
            case VK_DOWN:
            case VK_RCONTROL:
            case VK_RMENU:
            case VK_LWIN:
            case VK_RWIN:
            case VK_APPS:
            case VK_PRIOR:
            case VK_NEXT:
            case VK_END:
            case VK_HOME:
            case VK_INSERT:
            case VK_DELETE:
            case VK_DIVIDE:
            case VK_NUMLOCK:
                input.ki.dwFlags |= KF_EXTENDED;
                break;
        }
        if (keyEvent == KeyboardHook::KeyUp) input.ki.dwFlags |= KEYEVENTF_KEYUP;

        input.type           = INPUT_KEYBOARD;
        input.ki.wVk         = keys[i].getKeyCode();
        input.ki.wScan       = MapVirtualKey(keys[i].getKeyCode(), MAPVK_VK_TO_VSC);
        input.ki.dwExtraInfo = m_Hook->getIdentifier();
    }
    // BlockInput(true);
    int inputsSentCount = SendInput(inputsCount, inputs, sizeof(INPUT));
    // BlockInput(false);
    if (inputsSentCount != 0)
        qDebug() << qPrintable(QString("[%1]: Successfully sent %2 inputs -> [%3]: %4")
                                   .arg(QuickCut::fromKey(keyEvent), 8, QChar(' '))
                                   .arg(inputsSentCount)
                                   .arg(Action::getKeysCode(keys))
                                   .arg(Action::getKeysNames(keys).join('+')));

    // If it's a KeyPress, then we know the previous simulated keys were a KeyDown, so
    // we need to simulate a KeyUp right after KeyDown.
    if (keyEvent == KeyboardHook::KeyPress)
    {
        for (int i = 0; i < inputsCount; ++i) inputs[i].ki.dwFlags |= KEYEVENTF_KEYUP;
        SendInput(inputsCount, inputs, sizeof(INPUT));
    }

    delete[] inputs;
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
