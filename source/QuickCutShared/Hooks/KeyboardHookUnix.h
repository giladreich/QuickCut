
#pragma once

#include "KeyboardHook.h"

class KeyboardHookUnix : public KeyboardHook
{
    Q_OBJECT
public:
    KeyboardHookUnix(bool multiShortcuts, QObject * parent = nullptr);
    ~KeyboardHookUnix();

    bool activateHook() override;
    bool deactivateHook() override;

private:
};