
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyboardHook.h"

KeyboardHook * KeyboardHook::s_Instance = nullptr;

KeyboardHook::KeyboardHook(bool multiShortcuts, bool autoRepeatEnabled, QObject * parent)
    : QObject(parent)
    , m_MultiShortcuts(multiShortcuts)
    , m_AutoRepeatEnabled(autoRepeatEnabled)
{
    s_Instance   = this;
    m_Identifier = std::hash<const char *>{}(QUICKCUT_NAME QUICKCUT_VERSION);
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

bool KeyboardHook::isAutoRepeatEnabled() const
{
    return m_AutoRepeatEnabled;
}

void KeyboardHook::setAutoRepeatEnabled(bool autoRepeatEnabled)
{
    m_AutoRepeatEnabled = autoRepeatEnabled;
}

const size_t & KeyboardHook::getIdentifier() const
{
    return m_Identifier;
}
