
#include "pch.h"
#include "Action.h"
#include "Utils/Utility.h"

Action::Action() noexcept
    : BaseModel()
    , m_Type(ActionType::ActionUnknown)
    , m_SrcKey()
    , m_DstKey()
    , m_AppPath()
    , m_AppArgs()
    , m_CreatedDate(QuickCut::getDateTime())
{
}

Action::Action(const QString &    name,
               const ActionType & type,
               const QString &    srcKey,
               const QString &    dstKey,
               const QString &    appPath,
               const QString &    appArgs) noexcept
    : BaseModel(QuickCut::createUuid(), name, QuickCut::getDateTime())
    , m_Type(type)
    , m_SrcKey(srcKey)
    , m_DstKey(dstKey)
    , m_AppPath(appPath)
    , m_AppArgs(appArgs)
    , m_CreatedDate(QuickCut::getDateTime())
{
}

Action::Action(QString &&    name,
               ActionType && type,
               QString &&    srcKey,
               QString &&    dstKey,
               QString &&    appPath,
               QString &&    appArgs) noexcept
    : BaseModel(std::move(QuickCut::createUuid()),
                std::move(name),
                std::move(QuickCut::getDateTime()))
    , m_Type(std::move(type))
    , m_SrcKey(std::move(srcKey))
    , m_DstKey(std::move(dstKey))
    , m_AppPath(std::move(appPath))
    , m_AppArgs(std::move(appArgs))
    , m_CreatedDate(std::move(QuickCut::getDateTime()))
{
}

Action::Action(const QString &    id,
               const QString &    name,
               const QString &    lastModified,
               const ActionType & type,
               const QString &    srcKey,
               const QString &    dstKey,
               const QString &    appPath,
               const QString &    appArgs,
               const QString &    createdDate) noexcept
    : BaseModel(id, name, lastModified)
    , m_Type(type)
    , m_SrcKey(srcKey)
    , m_DstKey(dstKey)
    , m_AppPath(appPath)
    , m_AppArgs(appArgs)
    , m_CreatedDate(createdDate)
{
}

Action::Action(QString &&    id,
               QString &&    name,
               QString &&    lastModified,
               ActionType && type,
               QString &&    srcKey,
               QString &&    dstKey,
               QString &&    appPath,
               QString &&    appArgs,
               QString &&    createdDate) noexcept
    : BaseModel(std::move(id), std::move(name), std::move(lastModified))
    , m_Type(std::move(type))
    , m_SrcKey(std::move(srcKey))
    , m_DstKey(std::move(dstKey))
    , m_AppPath(std::move(appPath))
    , m_AppArgs(std::move(appArgs))
    , m_CreatedDate(std::move(createdDate))
{
}

QString Action::getType(ActionType type)
{
    switch (type)
    {
        case ActionType::ActionKeyMap:
            return "KeyMap";
        case ActionType::ActionAppStart:
            return "AppStart";

        case ActionType::ActionUnknown:
        default:
            return "Unknown";
    }
}

ActionType Action::getType(const QString & type)
{
    if (type == "KeyMap") return ActionType::ActionKeyMap;
    if (type == "AppStart") return ActionType::ActionAppStart;
    return ActionType::ActionUnknown;
}

QString Action::getKey(int key)
{
    return QString::number(key);
}

ActionType Action::getType() const
{
    return m_Type;
}

void Action::setType(ActionType type)
{
    m_Type = type;
}

QString Action::getSrcKey() const
{
    return m_SrcKey;
}

void Action::setSrcKey(const QString & key)
{
    m_SrcKey = key;
}

QString Action::getDstKey() const
{
    return m_DstKey;
}

void Action::setDstKey(const QString & key)
{
    m_DstKey = key;
}

const QString & Action::getAppPath() const
{
    return m_AppPath;
}

void Action::setAppPath(const QString & path)
{
    m_AppPath = path;
}

const QString & Action::getAppArgs() const
{
    return m_AppArgs;
}

void Action::setAppArgs(const QString & args)
{
    m_AppArgs = args;
}

const QString & Action::getCreatedDate() const
{
    return m_CreatedDate;
}

void Action::reset()
{
    m_Name.clear();
    m_Type = ActionUnknown;
    m_SrcKey.clear();
    m_DstKey.clear();
    m_AppPath.clear();
    m_AppArgs.clear();
}
