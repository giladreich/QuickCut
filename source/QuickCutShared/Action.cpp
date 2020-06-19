
#include "Action.h"
#include "pch.h"
#include <utility>

Action::Action()
    : m_Uuid(createUuid())
    , m_Name("")
    , m_Type(ActionUnknown)
    , m_SrcKey("")
    , m_DstKey("")
    , m_AppPath("")
    , m_AppArgs("")
    , m_CreatedDate(getDateTime())
{
}

Action::Action(std::string         name,
               const eActionType & type,
               std::string         srcKey,
               std::string         dstKey,
               std::string         appPath,
               std::string         appArgs)
    : m_Uuid(createUuid())
    , m_Name(std::move(name))
    , m_Type(type)
    , m_SrcKey(std::move(srcKey))
    , m_DstKey(std::move(dstKey))
    , m_AppPath(std::move(appPath))
    , m_AppArgs(std::move(appArgs))
    , m_CreatedDate(getDateTime())
{
}

Action::Action(std::string         id,
               std::string         name,
               const eActionType & type,
               std::string         srcKey,
               std::string         dstKey,
               std::string         appPath,
               std::string         appArgs,
               std::string         createdDate)
    : m_Uuid(std::move(id))
    , m_Name(std::move(name))
    , m_Type(type)
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
        case ActionKeyMap:
            return "KeyMap";
        case ActionAppStart:
            return "AppStart";

        case ActionUnknown:
        default:
            return "Unknown";
    }
}

eActionType Action::getType(const std::string & type)
{
    if (type == "KeyMap") return ActionKeyMap;
    if (type == "AppStart") return ActionAppStart;
    return ActionUnknown;
}

std::string Action::getKey(int key)
{
    return std::to_string(key);
}

const std::string & Action::getId() const
{
    return m_Uuid;
}

const std::string & Action::getName() const
{
    return m_Name;
}

void Action::setName(const std::string & name)
{
    m_Name = name;
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
