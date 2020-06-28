
#pragma once

#include "KeyboardHook.h"

#include <Windows.h>

class KeyboardHookWindows : public KeyboardHook
{
    Q_OBJECT
public:
    KeyboardHookWindows(bool      multiShortcuts,
                        bool      autoRepeatEnabled,
                        QObject * parent = nullptr);
    ~KeyboardHookWindows();

    bool activateHook() override;
    bool deactivateHook() override;

private:
    static LRESULT CALLBACK SysKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static KeyboardKeys     getKeysData(const QVector<KBDLLHOOKSTRUCT> & pressedKeys);
    static QString          mapMissingKeyName(const int key);

private:
    static HHOOK s_Hook;
};
