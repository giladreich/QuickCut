
#pragma once

#include <QObject>

class KeyboardHook : public QObject
{
    Q_OBJECT
public:
    virtual bool activateHook();
    virtual bool deactivateHook();

    // Only update this instance if multiple instances are created, so it knows to which
    // instance to send the signal keysPressed. Note that this is limit to run for only
    // one instance at a time. However, it's possible to store instances in a vector, using
    // the observer pattern, but there isn't really a need for this now.
    void setInstance(KeyboardHook * instance);

    bool isMultiShortcutsEnabled() const;
    void setMultiShortcuts(bool multiShortcuts);

signals:
    void keysPressed(const QStringList & keys, bool * outSwallowKey);

protected:
    KeyboardHook(bool multiShortcuts, QObject * parent = nullptr);
    virtual ~KeyboardHook();

protected:
    static KeyboardHook * s_Instance;

    bool m_MultiShortcuts;
};