
#include "QuickCutShared/QuickCutPCH.h"
#include "KeyData.h"

KeyData::KeyData() noexcept
    : m_KeyName()
    , m_KeyCode(0)
{
}

KeyData::KeyData(const QString & keyName, const int & keyCode) noexcept
    : m_KeyName(keyName)
    , m_KeyCode(keyCode)
{
}

KeyData::KeyData(const QString & keyName, const QString & keyCodeHex) noexcept
    : m_KeyName(keyName)
    , m_KeyCode(0)
{
    setKeyCodeHex(keyCodeHex);
}

KeyData::KeyData(QString && keyName, int && keyCode) noexcept
    : m_KeyName(std::move(keyName))
    , m_KeyCode(std::move(keyCode))
{
}

KeyData::KeyData(QString && keyName, QString && keyCodeHex) noexcept
    : m_KeyName(std::move(keyName))
    , m_KeyCode(0)
{
    setKeyCodeHex(keyCodeHex);
}

const QString & KeyData::getKeyName() const
{
    return m_KeyName;
}

void KeyData::setKeyName(const QString & keyName)
{
    m_KeyName = keyName;
}

QString KeyData::getKeyCodeHex() const
{
    return QString("%1").arg(m_KeyCode, 2, 16, QLatin1Char('0')).toUpper().prepend("0x");
}

void KeyData::setKeyCodeHex(const QString & hex)
{
    bool ok;
    int  keyCode = hex.toInt(&ok, 16);
    if (!ok)
    {
        qDebug() << "[KeyData::setKeyCodeHex]: Failed to convert " << qPrintable(hex)
                 << " to int.";
        return;
    }
    setKeyCode(keyCode);
}

int KeyData::getKeyCode() const
{
    return m_KeyCode;
}

void KeyData::setKeyCode(int keyCode)
{
    m_KeyCode = keyCode;
}
