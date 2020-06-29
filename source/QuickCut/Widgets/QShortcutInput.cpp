
#include "QuickCutShared/QuickCutPCH.h"
#include "QShortcutInput.h"

QShortcutInput::QShortcutInput(QWidget * parent)
    : QLineEdit(parent)
    , m_CurrentKeys(nullptr)
{
    // Note that multiShortcuts custom property is still false during constructions, therefore
    // we also call to set it every time we activate the hook.
    bool multiShortcuts    = property("multiShortcuts").toBool();
    bool autoRepeatEnabled = property("autoRepeatEnabled").toBool();

#if defined(Q_OS_WINDOWS)
    m_Hook = new KeyboardHookWindows(multiShortcuts, autoRepeatEnabled, this);
#elif defined(Q_OS_UNIX)
    m_Hook = new KeyboardHookUnix(multiShortcuts, autoRepeatEnabled, this);
#endif

    connect(m_Hook, &KeyboardHook::keysPressed, this, &QShortcutInput::onKeysPress);
}

QShortcutInput::~QShortcutInput()
{
    m_Hook->deactivateHook();
}

void QShortcutInput::focusInEvent(QFocusEvent * event)
{
    m_Hook->setInstance(m_Hook);
    m_Hook->setMultiShortcuts(property("multiShortcuts").toBool());
    m_Hook->activateHook();
}

void QShortcutInput::focusOutEvent(QFocusEvent * event)
{
    m_Hook->deactivateHook();
}

void QShortcutInput::onKeysPress(const KeyboardKeys & keys, bool * outSwallowKey)
{
    if (keys.isEmpty()) return;

    // Always save the current keys, so the UI can know whether new keys were set.
    // m_CurrentKeys = std::make_shared<KeyboardKeys>(keys);

    // Don't process any keyboard inputs globally when receiving inputs.
    // i.e. so keys like Super/WinKey won't pop-up a menu while it gets inputs.
    *outSwallowKey = true;

    QString text;
    for (auto && key : keys)
    {
        QString keyName = key.getKeyName();
        text += QString("%1+").arg(keyName);
    }
    text = text.left(text.length() - 1);

    setText(text);
}