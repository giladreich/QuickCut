
#pragma once

#include <string>

enum eActionType
{
    ActionKeyMap   = 0x00,
    ActionAppStart = 0x01,
    ActionUnknown  = 0xFF
};

class Action
{
public:
    Action();
    Action(std::string         name,
           const eActionType & type,
           std::string         srcKey,
           std::string         dstKey,
           std::string         appPath,
           std::string         appArgs);
    Action(std::string         id,
           std::string         name,
           const eActionType & type,
           std::string         srcKey,
           std::string         dstKey,
           std::string         appPath,
           std::string         appArgs,
           std::string         createdDate);

    static std::string getType(eActionType type);
    static eActionType getType(const std::string & type);
    static std::string getKey(int key);

    const std::string & getId() const;

    const std::string & getName() const;
    void                setName(const std::string & name);

    eActionType getType() const;
    void        setType(eActionType type);

    std::string getSrcKey() const;
    void        setSrcKey(const std::string & key);

    std::string getDstKey() const;
    void        setDstKey(const std::string & key);

    const std::string & getAppPath() const;
    void                setAppPath(const std::string & path);

    const std::string & getAppArgs() const;
    void                setAppArgs(const std::string & path);

    const std::string & getCreatedDate() const;

    void reset();

private:
    std::string m_szUuid;
    std::string m_szName;
    eActionType m_eType;
    std::string m_szSrcKey; // string with delimited ',' char. Could have multiple keys.
    std::string m_szDstKey;
    std::string m_szAppPath;
    std::string m_szAppArgs;
    std::string m_szCreatedDate;
};