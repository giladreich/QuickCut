
#pragma once

#include <QString>

class KeyData
{
public:
    // Constructs new profile
    KeyData() noexcept;

    // Constructs existing key data
    KeyData(const QString & keyName, const int & keyCode) noexcept;
    KeyData(const QString & keyName, const QString & keyCodeHex) noexcept;

    // Constructs existing key data
    KeyData(QString && keyName, int && keyCode) noexcept;
    KeyData(QString && keyName, QString && keyCodeHex) noexcept;

    // Constructs copy
    KeyData(const KeyData & profile) = default;
    KeyData(KeyData && profile)      = default;

    ~KeyData() = default;

    const QString & getKeyName() const;
    void            setKeyName(const QString & keyName);

    QString getKeyCodeHex() const;
    void    setKeyCodeHex(const QString & hex);
    int     getKeyCode() const;
    void    setKeyCode(int keyCode);

    KeyData & operator=(const KeyData & profile) = default;
    KeyData & operator=(KeyData && profile) = default;

private:
    QString m_KeyName;
    int     m_KeyCode;

    // TODO(Gilad): Consider unifying all platforms key codes to custom key code like Qt did.
    // KeyMap  m_MyMappedKey;
};
