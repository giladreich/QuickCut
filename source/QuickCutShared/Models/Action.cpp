
#include "QuickCutShared/QuickCutPCH.h"
#include "Action.h"

Action::Action() noexcept
    : BaseModel()
    , m_Type(Action::ActionKeyMap)
    , m_SrcKeys()
    , m_DstKeys()
    , m_TargetPath()
    , m_AppArgs()
    , m_AutoText()
    , m_CreatedDate(QuickCut::getDateTime())
    , m_Enabled(true)
{
}

Action::Action(const QString & id,
               const QString & lastModified,
               const QString & createdDate) noexcept
    : BaseModel(id, lastModified)
    , m_Type(ActionType::ActionInvalid)
    , m_SrcKeys()
    , m_DstKeys()
    , m_TargetPath()
    , m_AppArgs()
    , m_AutoText()
    , m_CreatedDate(createdDate)
    , m_Enabled(true)
{
}

Action::Action(QString && id, QString && lastModified, QString && createdDate) noexcept
    : BaseModel(std::move(id), std::move(lastModified))
    , m_Type(ActionType::ActionInvalid)
    , m_SrcKeys()
    , m_DstKeys()
    , m_TargetPath()
    , m_AppArgs()
    , m_AutoText()
    , m_CreatedDate(std::move(createdDate))
    , m_Enabled(true)
{
}

Action::ActionType Action::getType() const
{
    return m_Type;
}

QString Action::getTypeName() const
{
    return Action::getTypeName(m_Type);
}

QString Action::getTypeName(ActionType type)
{
    if (type == Action::ActionKeyMap)
        return "Key Mapping";
    else if (type == Action::ActionAppLaunch)
        return "Open Application";
    else if (type == Action::ActionDirLaunch)
        return "Open Directory";
    else if (type == Action::ActionAutoText)
        return "Auto Text";

    return {};
}

void Action::setType(ActionType type)
{
    if (type != Action::ActionInvalid)
    {
        m_Type         = type;
        m_LastModified = QuickCut::getDateTime();
    }
}

const QString & Action::getTargetPath() const
{
    return m_TargetPath;
}

void Action::setTargetPath(const QString & path)
{
    m_TargetPath   = path;
    m_LastModified = QuickCut::getDateTime();
}

const QString & Action::getAppArgs() const
{
    return m_AppArgs;
}

void Action::setAppArgs(const QString & args)
{
    m_AppArgs      = args;
    m_LastModified = QuickCut::getDateTime();
}

const QString & Action::getAutoText() const
{
    return m_AutoText;
}

void Action::setAutoText(const QString & text)
{
    m_AutoText     = text;
    m_LastModified = QuickCut::getDateTime();
}

const QString & Action::getCreatedDate() const
{
    return m_CreatedDate;
}

bool Action::isEnabled() const
{
    return m_Enabled;
}

void Action::setEnabled(bool enabled)
{
    m_Enabled = enabled;
}

void Action::reset()
{
    m_Name.clear();
    m_LastModified = QuickCut::getDateTime();
    m_Type         = Action::ActionKeyMap;
    m_SrcKeys.clear();
    m_DstKeys.clear();
    m_TargetPath.clear();
    m_AppArgs.clear();
}

const KeyboardKeys & Action::getSrcKeys() const
{
    return m_SrcKeys;
}

void Action::setSrcKeys(const KeyboardKeys & keys)
{
    m_SrcKeys      = keys;
    m_LastModified = QuickCut::getDateTime();
}

QStringList Action::getSrcKeysNames() const
{
    return getKeysNames(m_SrcKeys);
}

QString Action::getSrcKeysName(QChar delimiter) const
{
    return getKeysNames(m_SrcKeys).join(delimiter);
}

QString Action::getSrcKeysCode() const
{
    return getKeysCode(m_SrcKeys);
}

const KeyboardKeys & Action::getDstKeys() const
{
    return m_DstKeys;
}

void Action::setDstKeys(const KeyboardKeys & keys)
{
    m_DstKeys      = keys;
    m_LastModified = QuickCut::getDateTime();
}

QStringList Action::getDstKeysNames() const
{
    return getKeysNames(m_DstKeys);
}

QString Action::getDstKeysName(QChar delimiter) const
{
    return getKeysNames(m_DstKeys).join(delimiter);
}

QString Action::getDstKeysCode() const
{
    return getKeysCode(m_DstKeys);
}

QStringList Action::getKeysNames(const KeyboardKeys & keys)
{
    QStringList names;
    for (auto && key : keys) names << key.getKeyName();
    return names;
}

QString Action::getKeysCode(const KeyboardKeys & keys)
{
    QString codes;
    for (auto && key : keys) codes += QString::number(key.getKeyCode(), 16);
    return codes.toUpper();
}
