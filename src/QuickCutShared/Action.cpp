
#include "Action.h"
#include "pch.h"
#include <utility>


Action::Action()
    : m_szUuid(createUuid())
    , m_szName("")
    , m_eType(ActionUnknown)
    , m_szSrcKey("")
    , m_szDstKey("")
    , m_szAppPath("")
    , m_szAppArgs("")
    , m_szCreatedDate(getDateTime())
{

}

Action::Action(std::string name, const eActionType & type,
              std::string srcKey, std::string dstKey,
              std::string appPath, std::string appArgs)
    : m_szUuid(createUuid())
    , m_szName(std::move(name))
    , m_eType(type)
    , m_szSrcKey(std::move(srcKey))
    , m_szDstKey(std::move(dstKey))
    , m_szAppPath(std::move(appPath))
    , m_szAppArgs(std::move(appArgs))
    , m_szCreatedDate(getDateTime())
{

}

Action::Action(std::string id, std::string name, const eActionType & type,
               std::string srcKey, std::string dstKey,
               std::string appPath, std::string appArgs,
               std::string createdDate)
    : m_szUuid(std::move(id))
    , m_szName(std::move(name))
    , m_eType(type)
    , m_szSrcKey(std::move(srcKey))
    , m_szDstKey(std::move(dstKey))
    , m_szAppPath(std::move(appPath))
    , m_szAppArgs(std::move(appArgs))
    , m_szCreatedDate(std::move(createdDate))
{
}

std::string Action::getType(eActionType type)
{
    switch (type)
    {
        case ActionKeyMap: return "KeyMap";
        case ActionAppStart: return "AppStart";
        
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
    return m_szUuid;
}

const std::string & Action::getName() const
{
    return m_szName;
}

void Action::setName(const std::string & name)
{
    m_szName = name;
}

eActionType Action::getType() const
{
    return m_eType;
}

void Action::setType(eActionType type)
{
    m_eType = type;
}

std::string Action::getSrcKey() const
{
    return m_szSrcKey;
}

void Action::setSrcKey(const std::string & key)
{
    m_szSrcKey = key;
}

std::string Action::getDstKey() const
{
    return m_szDstKey;
}

void Action::setDstKey(const std::string & key)
{
    m_szDstKey = key;
}

const std::string & Action::getAppPath() const
{
    return m_szAppPath;
}

void Action::setAppPath(const std::string & path)
{
    m_szAppPath = path;
}

const std::string & Action::getAppArgs() const
{
    return m_szAppArgs;
}

void Action::setAppArgs(const std::string & args)
{
    m_szAppArgs = args;
}

const std::string & Action::getCreatedDate() const
{
    return m_szCreatedDate;
}

void Action::reset()
{
    m_szName.clear();
    m_eType = ActionUnknown;
    m_szSrcKey.clear();
    m_szDstKey.clear();
    m_szAppPath.clear();
    m_szAppArgs.clear();
}
