
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyboardHook.h"

KeyboardHook * KeyboardHook::s_Instance = nullptr;

KeyboardHook::KeyboardHook(bool multiShortcuts, QObject * parent)
    : QObject(parent)
    , m_MultiShortcuts(multiShortcuts)
{
    s_Instance = this;
}

KeyboardHook::~KeyboardHook() = default;

bool KeyboardHook::activateHook()
{
    return true;
}

bool KeyboardHook::deactivateHook()
{
    return true;
}

void KeyboardHook::setInstance(KeyboardHook * instance)
{
    s_Instance = instance;
}

bool KeyboardHook::isMultiShortcutsEnabled() const
{
    return m_MultiShortcuts;
}

void KeyboardHook::setMultiShortcuts(bool multiShortcuts)
{
    m_MultiShortcuts = multiShortcuts;
}
