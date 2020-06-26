
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

public:
    QShortcutInput(QWidget * parent);
    ~QShortcutInput();

    void focusInEvent(QFocusEvent * event) override;
    void focusOutEvent(QFocusEvent * event) override;

public slots:
    void onKeysPress(const QStringList & keys, bool * outSwallowKey);

public:
    std::shared_ptr<QStringList> m_CurrentKeys;

private:
    QPointer<KeyboardHook> m_Hook;
};