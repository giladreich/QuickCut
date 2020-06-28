
#pragma once

#include "QuickCutShared/Models/KeyData.h"

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

    bool isAutoRepeatEnabled() const;
    void setAutoRepeatEnabled(bool autoRepeatEnabled);

    // Returns a unique identifier for validating message exchanges when sending inputs.
    const size_t & getIdentifier() const;

signals:
    void keysPressed(const KeyboardKeys & keys, bool * outSwallowKey);

protected:
    KeyboardHook(bool multiShortcuts, bool autoRepeatEnabled, QObject * parent = nullptr);
    virtual ~KeyboardHook();

protected:
    static KeyboardHook * s_Instance;

    size_t m_Identifier;
    bool   m_MultiShortcuts;
    bool   m_AutoRepeatEnabled;
};
