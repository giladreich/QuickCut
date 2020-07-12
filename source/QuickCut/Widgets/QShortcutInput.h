
#pragma once

#if defined(Q_OS_WINDOWS)
#    include "QuickCutShared/Hooks/KeyboardHookWindows.h"
#elif defined(Q_OS_UNIX)
#    include "QuickCutShared/Hooks/KeyboardHookUnix.h"
#endif

#include <QLineEdit>

class QShortcutInput : public QLineEdit
{
    Q_OBJECT

    // Q_PROPERTY(bool multiShortcuts READ isMultiShortcuts WRITE setMultiShortcuts)
    // Q_PROPERTY(bool autoRepeatEnabled READ isAutoRepeatEnabled WRITE setAutoRepeatEnabled)
public:
    QShortcutInput(QWidget * parent);
    ~QShortcutInput();

    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;

public slots:
    void onKeysDown(const KeyboardKeys & keys, bool * outSwallowKey);

public:
    std::unique_ptr<KeyboardKeys> m_CurrentKeys;

private:
    QPointer<KeyboardHook> m_Hook;
};