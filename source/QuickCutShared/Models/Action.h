
#pragma once

#include "BaseModel.h"

enum eActionType
{
    ActionKeyMap = 0,
    ActionAppStart,
    ActionUnknown = 0xFF
};

class Action : public BaseModel
{
public:
    // Constructs new Action
    Action() noexcept;

    // Constructs new Action
    Action(const std::string & name,
           const eActionType & type,
           const std::string & srcKey,
           const std::string & dstKey,
           const std::string & appPath,
           const std::string & appArgs) noexcept;

    Action(std::string && name,
           eActionType && type,
           std::string && srcKey,
           std::string && dstKey,
           std::string && appPath,
           std::string && appArgs) noexcept;

    // Constructs existing Action
    Action(const std::string & id,
           const std::string & name,
           const std::string & lastModified,
           const eActionType & type,
           const std::string & srcKey,
           const std::string & dstKey,
           const std::string & appPath,
           const std::string & appArgs,
           const std::string & createdDate) noexcept;

    Action(std::string && id,
           std::string && name,
           std::string && lastModified,
           eActionType && type,
           std::string && srcKey,
           std::string && dstKey,
           std::string && appPath,
           std::string && appArgs,
           std::string && createdDate) noexcept;

    // Constructs copy
    Action(const Action & action) = default;
    Action(Action && action)      = default;

    static std::string getType(eActionType type);
    static eActionType getType(const std::string & type);
    static std::string getKey(int key);

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

    Action & operator=(const Action & action) = default;
    Action & operator=(Action && action) = default;

private:
    eActionType m_Type;
    std::string m_SrcKey; // string with delimited ',' char. Could have multiple keys.
    std::string m_DstKey;
    std::string m_AppPath;
    std::string m_AppArgs;
    std::string m_CreatedDate;
};