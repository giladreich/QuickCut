
#include "QShortcutInput.h"

#include <QDebug>

QShortcutInput * QShortcutInput::s_pInstance = nullptr;

#if defined(Q_OS_WIN)
HHOOK QShortcutInput::s_hHook = nullptr;
#endif

QShortcutInput::QShortcutInput(QWidget * parent)
    : QLineEdit(parent)
{
}

QShortcutInput::~QShortcutInput()
{
#if defined(Q_OS_WIN)
    if (s_hHook)
    {
        qDebug() << "[QShortcutInput::dtor] - Unhooking...";
        UnhookWindowsHookEx(s_hHook);
        s_pInstance = nullptr;
        s_hHook     = nullptr;
    }
#endif
}

#if defined(Q_OS_WIN)
LRESULT CALLBACK QShortcutInput::WndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || !s_pInstance) return CallNextHookEx(s_hHook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT * pKbd = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    // Workaround for auto-repeat, since low level hook doesn't provide KF_REPEAT flags in
    // lParam: (lParam & KF_REPEAT)
    static DWORD dwPrevVkCode = 0;

    static QString szKeys;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        if (dwPrevVkCode == pKbd->vkCode) return -1;

        dwPrevVkCode = pKbd->vkCode;

        if (s_pInstance->property("multiShortcuts").toBool())
            szKeys += QString::number(pKbd->vkCode, 16);
        else
            szKeys = QString::number(pKbd->vkCode, 16);

        s_pInstance->setText(szKeys);

        return -1;
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        szKeys.clear();

        dwPrevVkCode = 0;

        return -1;
    }

    return CallNextHookEx(s_hHook, nCode, wParam, lParam);
}
#endif // #if defined(Q_OS_WIN)

#if defined(Q_OS_WIN)
void QShortcutInput::focusInEvent(QFocusEvent * event)
{
    if (!s_hHook)
    {
        qDebug() << "[QShortcutInput::focusInEvent] - Hooking...";
        s_pInstance = this;
        s_hHook     = SetWindowsHookEx(WH_KEYBOARD_LL, WndProc, nullptr, 0);
        if (!s_hHook) { qDebug() << "[QShortcutInput::focusInEvent] - Hook failed..."; }
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
    if (s_hHook)
    {
        qDebug() << "[QShortcutInput::focusInEvent] - Unhooking...";
        UnhookWindowsHookEx(s_hHook);
        s_pInstance = nullptr;
        s_hHook     = nullptr;
    }
}
#elif defined(Q_OS_UNIX)
void QShortcutInput::focusOutEvent(QFocusEvent * event)
{
    // TODO: Add unix hook.
}
#endif // #if defined(Q_OS_WIN)
