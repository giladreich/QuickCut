
#pragma once

#include "KeyboardHook.h"

class KeyboardHookUnix : public KeyboardHook
{
    Q_OBJECT
public:
    KeyboardHookUnix(bool multiShortcuts, bool autoRepeatEnabled, QObject * parent = nullptr);
    ~KeyboardHookUnix();

    bool activateHook() override;
    bool deactivateHook() override;
};
