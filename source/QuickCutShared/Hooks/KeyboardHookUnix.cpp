
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyboardHookUnix.h"

KeyboardHookUnix::KeyboardHookUnix(bool multiShortcuts, QObject * parent)
    : KeyboardHook(multiShortcuts, parent)
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
