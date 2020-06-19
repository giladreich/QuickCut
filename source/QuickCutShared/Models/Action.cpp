
#include "Action.h"
#include "pch.h"

#include <utility>

Action::Action() noexcept
    : BaseModel()
    , m_Type(eActionType::ActionUnknown)
    , m_SrcKey()
    , m_DstKey()
    , m_AppPath()
    , m_AppArgs()
    , m_CreatedDate(getDateTime())
{
}

Action::Action(const std::string & name,
               const eActionType & type,
               const std::string & srcKey,
               const std::string & dstKey,
               const std::string & appPath,
               const std::string & appArgs) noexcept
    : BaseModel(createUuid(), name, getDateTime())
    , m_Type(type)
    , m_SrcKey(srcKey)
    , m_DstKey(dstKey)
    , m_AppPath(appPath)
    , m_AppArgs(appArgs)
    , m_CreatedDate(getDateTime())
{
}

Action::Action(std::string && name,
               eActionType && type,
               std::string && srcKey,
               std::string && dstKey,
               std::string && appPath,
               std::string && appArgs) noexcept
    : BaseModel(std::move(createUuid()), std::move(name), std::move(getDateTime()))
    , m_Type(std::move(type))
    , m_SrcKey(std::move(srcKey))
    , m_DstKey(std::move(dstKey))
    , m_AppPath(std::move(appPath))
    , m_AppArgs(std::move(appArgs))
    , m_CreatedDate(std::move(getDateTime()))
{
}

Action::Action(const std::string & id,
               const std::string & name,
               const std::string & lastModified,
               const eActionType & type,
               const std::string & srcKey,
               const std::string & dstKey,
               const std::string & appPath,
               const std::string & appArgs,
               const std::string & createdDate) noexcept
    : BaseModel(id, name, lastModified)
    , m_Type(type)
    , m_SrcKey(srcKey)
    , m_DstKey(dstKey)
    , m_AppPath(appPath)
    , m_AppArgs(appArgs)
    , m_CreatedDate(createdDate)
{
}

Action::Action(std::string && id,
               std::string && name,
               std::string && lastModified,
               eActionType && type,
               std::string && srcKey,
               std::string && dstKey,
               std::string && appPath,
               std::string && appArgs,
               std::string && createdDate) noexcept
    : BaseModel(std::move(id), std::move(name), std::move(lastModified))
    , m_Type(std::move(type))
    , m_SrcKey(std::move(srcKey))
    , m_DstKey(std::move(dstKey))
    , m_AppPath(std::move(appPath))
    , m_AppArgs(std::move(appArgs))
    , m_CreatedDate(std::move(createdDate))
{
}

std::string Action::getType(eActionType type)
{
    switch (type)
    {
        case eActionType::ActionKeyMap:
            return "KeyMap";
        case eActionType::ActionAppStart:
            return "AppStart";

        case eActionType::ActionUnknown:
        default:
            return "Unknown";
    }
}

eActionType Action::getType(const std::string & type)
{
    if (type == "KeyMap") return eActionType::ActionKeyMap;
    if (type == "AppStart") return eActionType::ActionAppStart;
    return eActionType::ActionUnknown;
}

std::string Action::getKey(int key)
{
    return std::to_string(key);
}

eActionType Action::getType() const
{
    return m_Type;
}

void Action::setType(eActionType type)
{
    m_Type = type;
}

std::string Action::getSrcKey() const
{
    return m_SrcKey;
}

void Action::setSrcKey(const std::string & key)
{
    m_SrcKey = key;
}

std::string Action::getDstKey() const
{
    return m_DstKey;
}

void Action::setDstKey(const std::string & key)
{
    m_DstKey = key;
}

const std::string & Action::getAppPath() const
{
    return m_AppPath;
}

void Action::setAppPath(const std::string & path)
{
    m_AppPath = path;
}

const std::string & Action::getAppArgs() const
{
    return m_AppArgs;
}

void Action::setAppArgs(const std::string & args)
{
    m_AppArgs = args;
}

const std::string & Action::getCreatedDate() const
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
