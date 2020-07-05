
#include "QuickCutShared/QuickCutPCH.h"
#include "QShortcutInput.h"

QShortcutInput * QShortcutInput::s_Instance = nullptr;

#if defined(Q_OS_WIN)
HHOOK QShortcutInput::s_Hook = nullptr;
#endif

QShortcutInput::QShortcutInput(QWidget * parent)
    : QLineEdit(parent)
{
}

QShortcutInput::~QShortcutInput()
{
#if defined(Q_OS_WIN)
    if (s_Hook)
    {
        qDebug() << "[QShortcutInput::dtor] - Unhooking...";
        UnhookWindowsHookEx(s_Hook);
        s_Instance = nullptr;
        s_Hook     = nullptr;
    }
#endif
}

#if defined(Q_OS_WIN)
LRESULT CALLBACK QShortcutInput::WndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || !s_Instance) return CallNextHookEx(s_Hook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT * kbd = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    // Workaround for auto-repeat, since low level hook doesn't provide KF_REPEAT flags in
    // lParam: (lParam & KF_REPEAT)
    static DWORD prevVkCode = 0;

    static QString pressedKeys;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        if (prevVkCode == kbd->vkCode) return -1;

        prevVkCode = kbd->vkCode;

        if (s_Instance->property("multiShortcuts").toBool())
            pressedKeys += QString::number(kbd->vkCode, 16);
        else
            pressedKeys = QString::number(kbd->vkCode, 16);

        s_Instance->setText(pressedKeys);

        return -1;
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        pressedKeys.clear();

        prevVkCode = 0;

        return -1;
    }

    return CallNextHookEx(s_Hook, nCode, wParam, lParam);
}
#endif // #if defined(Q_OS_WIN)

#if defined(Q_OS_WIN)
void QShortcutInput::focusInEvent(QFocusEvent * event)
{
    if (!s_Hook)
    {
        qDebug() << "[QShortcutInput::focusInEvent] - Hooking...";
        s_Instance = this;
        s_Hook     = SetWindowsHookEx(WH_KEYBOARD_LL, WndProc, nullptr, 0);
        if (!s_Hook) { qDebug() << "[QShortcutInput::focusInEvent] - Hook failed..."; }
    }
}
#elif defined(Q_OS_UNIX)
void QShortcutInput::focusInEvent(QFocusEvent * event)
{
    // TODO: Add unix hook.
}
#endif // #if defined(Q_OS_WIN)

#if defined(Q_OS_WIN)
void QShortcutInput::focusOutEvent(QFocusEvent * event)
{
    if (s_Hook)
    {
        qDebug() << "[QShortcutInput::focusInEvent] - Unhooking...";
        UnhookWindowsHookEx(s_Hook);
        s_Instance = nullptr;
        s_Hook     = nullptr;
    }
}
#elif defined(Q_OS_UNIX)
void QShortcutInput::focusOutEvent(QFocusEvent * event)
{
    // TODO: Add unix hook.
}
#endif // #if defined(Q_OS_WIN)
