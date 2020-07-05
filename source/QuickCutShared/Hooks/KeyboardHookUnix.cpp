
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyboardHookUnix.h"

KeyboardHookUnix::KeyboardHookUnix(bool      multiShortcuts,
                                   bool      autoRepeatEnabled,
                                   QObject * parent)
    : KeyboardHook(multiShortcuts, autoRepeatEnabled, parent)
{
}

KeyboardHookUnix::~KeyboardHookUnix()
{
    deactivateHook();
}

bool KeyboardHookUnix::activateHook()
{
    // TODO(Gilad): Implement.
    return true;
}

bool KeyboardHookUnix::deactivateHook()
{
    // TODO(Gilad): Implement.
    return true;
}
