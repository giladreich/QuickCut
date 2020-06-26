
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyboardHookWindows.h"

union KeyState
{
    LPARAM lParam;

    struct
    {
        unsigned nRepeatCount : 16;
        unsigned nScanCode : 8;
        unsigned nExtended : 1;
        unsigned nReserved : 4;
        unsigned nContext : 1;
        unsigned nPrev : 1;
        unsigned nTrans : 1;
    };
};

HHOOK KeyboardHookWindows::s_Hook = nullptr;

KeyboardHookWindows::KeyboardHookWindows(bool multiShortcuts, QObject * parent)
    : KeyboardHook(multiShortcuts, parent)
{
    s_Hook = nullptr;
}

KeyboardHookWindows::~KeyboardHookWindows()
{
    deactivateHook();
}

bool KeyboardHookWindows::activateHook()
{
    if (!s_Hook)
    {
        qDebug() << "[KeyboardHookWindows::activateHook] - Hooking...";
        s_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, SysKeyboardProc, nullptr, 0);
        if (!s_Hook)
        {
            qDebug()
                << "[KeyboardHookWindows::activateHook] - Failed to activate keyboard hook...";
            return false;
        }
    }

    return true;
}

bool KeyboardHookWindows::deactivateHook()
{
    if (s_Hook)
    {
        qDebug() << "[KeyboardHookWindows::deactivateHook] - Unhooking...";
        if (!UnhookWindowsHookEx(s_Hook))
        {
            qDebug()
                << "[KeyboardHookWindows::deactivateHook] - Failed to deactivate keyboard "
                   "hook...";
            s_Hook = nullptr;
            return false;
        }
        s_Hook = nullptr;
    }

    return true;
}

LRESULT CALLBACK KeyboardHookWindows::SysKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || !s_Instance) return CallNextHookEx(s_Hook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT * kbd = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    // If it's an input we sent by simulating keyboard presses, don't do anything.
    bool injectedKeys = kbd->flags & LLKHF_LOWER_IL_INJECTED || kbd->flags & LLKHF_INJECTED;
    if (injectedKeys) return CallNextHookEx(s_Hook, nCode, wParam, lParam);

    // Workaround for auto-repeat, since low level hook doesn't provide KF_REPEAT flags in
    // lParam: (lParam & KF_REPEAT)
    static DWORD prevVkCode = 0;

    static QVector<KBDLLHOOKSTRUCT> kbds;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        bool isRepeatedKeyPress = prevVkCode == kbd->vkCode;
        if (isRepeatedKeyPress) return -1;

        prevVkCode = kbd->vkCode;

        // Only allow processing user defined shortcuts if it's not auto-repeat key press to
        // prevent situations where if a shortcut like CTRL+F2 and the user press the CTRL key
        // too long, it would be CTRL+CTRL...+F2 and would mismatch the desired behavior.
        if (!isRepeatedKeyPress)
        {
            KBDLLHOOKSTRUCT kbdCopy;
            memcpy(&kbdCopy, kbd, sizeof(KBDLLHOOKSTRUCT));

            if (s_Instance->isMultiShortcutsEnabled()) { kbds.push_back(kbdCopy); }
            else
            {
                kbds.clear();
                kbds.push_back(kbdCopy);
            }

            bool swallowKey = false;
            s_Instance->keysPressed(getKeysData(kbds), &swallowKey);
            if (swallowKey) return -1;
        }
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        kbds.clear();

        prevVkCode = 0;
    }

    return CallNextHookEx(s_Hook, nCode, wParam, lParam);
}

QStringList KeyboardHookWindows::getKeysData(const QVector<KBDLLHOOKSTRUCT> & pressedKeys)
{
    if (pressedKeys.isEmpty()) return QStringList();

    QStringList keysData;
    for (auto && kbd : pressedKeys)
    {
        KeyState state;
        state.nRepeatCount = 0;
        state.nScanCode    = kbd.scanCode;
        state.nExtended    = (kbd.flags & LLKHF_EXTENDED) != 0;
        state.nReserved    = 0;
        state.nContext     = (kbd.flags & LLKHF_ALTDOWN) != 0;
        state.nPrev        = 0;
        state.nTrans       = (kbd.flags & LLKHF_UP) != 0;

        // UINT scanCode = MapVirtualKey(kbd.vkCode, MAPVK_VK_TO_VSC);
        // if ((kbd.flags & LLKHF_EXTENDED) != 0) scanCode |= KF_EXTENDED;
        // set the scanCode in the 16 bit position of the lParam.
        // LPARAM lParam = (scanCode << 16);

        TCHAR keyBuff[256]{};
        GetKeyNameText(state.lParam, reinterpret_cast<LPWSTR>(&keyBuff), sizeof(keyBuff));

        QString keyName = QString::fromWCharArray(keyBuff);
        if (keyName.isEmpty()) keyName = mapMissingKeyName(kbd.vkCode);
        keysData << keyName;
    }

    return keysData;
}

QString KeyboardHookWindows::mapMissingKeyName(const int key)
{
    QString result;
    switch (key)
    {
        case VK_RSHIFT:
            result = "Right Shift";
            break;
        case VK_F13:
            result = "F13";
            break;
        case VK_F14:
            result = "F14";
            break;
        case VK_F15:
            result = "F15";
            break;
        case VK_F16:
            result = "F16";
            break;
        case VK_F17:
            result = "F17";
            break;
        case VK_F18:
            result = "F18";
            break;
        case VK_F19:
            result = "F19";
            break;
        case VK_F20:
            result = "F20";
            break;
        case VK_F21:
            result = "F21";
            break;
        case VK_F22:
            result = "F22";
            break;
        case VK_F23:
            result = "F23";
            break;
        case VK_F24:
            result = "F24";
            break;
        default:
            QString unmappedkey = "0x";
            unmappedkey += QString("%1").arg(key, 2, 16, QLatin1Char('0')).toUpper();
            qDebug() << "Unmapped key found! Please report! -> " << unmappedkey;
            break;
    }

    return result;
}
