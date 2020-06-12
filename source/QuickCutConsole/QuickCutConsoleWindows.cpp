

#include "QuickCutConsoleWindows.h"
#include "pch.h"
#include "Profile.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

// The GUI will send this pattern of key codes to notify that profile changes has been made
// so it knows when to reload the profile data.
#define RESERVED_RELOAD_KEY "86878687"

#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01

HHOOK QuickCutConsoleWindows::s_hHook = nullptr;

QuickCutConsoleWindows::QuickCutConsoleWindows(int argc, char * argv[])
    : QuickCutConsole(argc, argv)
{
    s_pInstance = this;
}

QuickCutConsoleWindows::~QuickCutConsoleWindows()
{
    if (s_hHook)
    {
        qDebug() << "[QuickCutConsoleWindows::dtor] - Unhooking...";
        UnhookWindowsHookEx(s_hHook);
        s_hHook = nullptr;
    }
}

LRESULT CALLBACK QuickCutConsoleWindows::WndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0) return CallNextHookEx(s_hHook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT * pKbd = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    static byte    byKeys[256] = {0};
    static QString szPressedKeys;
    static bool    bKeysProcessed = false;

    // Workaround for auto-repeat, since low level hook doesn't provide KF_REPEAT flags in
    // lParam: (lParam & KF_REPEAT)
    static DWORD dwPrevVkCode = 0;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        if (dwPrevVkCode == pKbd->vkCode)
            return CallNextHookEx(s_hHook, nCode, wParam, lParam);

        dwPrevVkCode = pKbd->vkCode;

        if (bKeysProcessed)
        {
            szPressedKeys.clear();
            bKeysProcessed = false;
        }

        szPressedKeys += QString::number(pKbd->vkCode, 16);
        qDebug() << "Current Pressed Keys: " << szPressedKeys;

        byKeys[pKbd->vkCode] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
        printKeyName(pKbd);

        if (szPressedKeys == RESERVED_RELOAD_KEY)
        {
            loadProfiles();
            qDebug() << "Refresh signal requested. Reloading profiles.";
            return CallNextHookEx(s_hHook, nCode, wParam, lParam);
        }

        if (!s_pProfile) { return CallNextHookEx(s_hHook, nCode, wParam, lParam); }

        for (auto && action : s_pProfile->getActions())
        {
            if (szPressedKeys.toStdString() == action->getSrcKey())
            {
                qDebug() << "Pressed Keys Match!: " << szPressedKeys
                         << " | Actual Keys: " << QString::fromStdString(action->getSrcKey());
                eActionType eType = action->getType();
                if (eType == ActionKeyMap)
                {
                    qDebug() << "Mapping key -> " << szPressedKeys << " To -> "
                             << QString::fromStdString(action->getDstKey());
                    szPressedKeys
                        .clear(); // Make sure to clear keys before sending another key.
                    static int vkDstCode = 0;
                    vkDstCode = std::strtol(action->getDstKey().c_str(), nullptr, 16);

                    INPUT in  = {0};
                    in.type   = INPUT_KEYBOARD;
                    in.ki.wVk = vkDstCode;
                    SendInput(1, &in, sizeof(INPUT));

                    return -1; // Don't process the source input.
                }
                else if (eType == ActionAppStart)
                {
                    qDebug() << "Running process -> "
                             << QString::fromStdString(action->getAppPath()) << " With key -> "
                             << szPressedKeys;
                    executeProcess(action->getAppPath(), action->getAppArgs());
                }
            }
        } // end for
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        byKeys[pKbd->vkCode] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;

        if (!bKeysProcessed)
        {
            dwPrevVkCode   = 0;
            bKeysProcessed = true;
        }
    }

    return CallNextHookEx(s_hHook, nCode, wParam, lParam);
}

bool QuickCutConsoleWindows::isKeyDown(byte byKey)
{
    return ((byKey & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK);
}

void QuickCutConsoleWindows::printKeyName(KBDLLHOOKSTRUCT * pKbd)
{
    char  lpszName[256] = {0};
    DWORD dwMsg         = 1;
    dwMsg += pKbd->scanCode << 16;
    dwMsg += pKbd->flags << 24;
    GetKeyNameText(dwMsg, reinterpret_cast<LPTSTR>(lpszName), sizeof(lpszName));

    QString str;
    str.asprintf("ScanCode: %d | VirtualKey: 0x%02X | KeyName: ", pKbd->scanCode,
                 pKbd->vkCode);
    str += QString::fromUtf16(reinterpret_cast<ushort *>(lpszName));
    qDebug() << str;
}

bool QuickCutConsoleWindows::start()
{
    if (!QuickCutConsole::start())
    {
        qDebug() << "[QuickCutConsoleWindows::start] - Profiles file hasn't been loaded yet.";
    }

    if (!s_hHook)
    {
        qDebug() << "[QuickCutConsoleWindows::start] - Hooking...";
        s_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, WndProc, nullptr, 0);
        if (!s_hHook)
        {
            qDebug() << "[QuickCutConsoleWindows::start] - Hook failed...";
            return false;
        }
    }

    return true;
}

bool QuickCutConsoleWindows::stop()
{
    QuickCutConsole::stop();

    if (s_hHook)
    {
        qDebug() << "[QuickCutConsoleWindows::stop] - Unhooking...";
        UnhookWindowsHookEx(s_hHook);
        s_hHook = nullptr;
    }

    return true;
}
