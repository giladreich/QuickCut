
#pragma once

#include "BaseModel.h"

class Action : public BaseModel
{
    Q_GADGET
public:
    enum ActionType
    {
        ActionInvalid = -1,
        ActionKeyMap  = 0,
        ActionAppLaunch,

        ActionCount,
    };
    Q_ENUM(ActionType)

    // Constructs new Action
    Action() noexcept;

    // Constructs new Action
    Action(const QString &    name,
           const ActionType & type,
           const QString &    srcKey,
           const QString &    dstKey,
           const QString &    appPath,
           const QString &    appArgs) noexcept;

    Action(QString &&    name,
           ActionType && type,
           QString &&    srcKey,
           QString &&    dstKey,
           QString &&    appPath,
           QString &&    appArgs) noexcept;

    // Constructs existing Action
    Action(const QString &    id,
           const QString &    name,
           const QString &    lastModified,
           const ActionType & type,
           const QString &    srcKey,
           const QString &    dstKey,
           const QString &    appPath,
           const QString &    appArgs,
           const QString &    createdDate) noexcept;

    Action(QString &&    id,
           QString &&    name,
           QString &&    lastModified,
           ActionType && type,
           QString &&    srcKey,
           QString &&    dstKey,
           QString &&    appPath,
           QString &&    appArgs,
           QString &&    createdDate) noexcept;

    // Constructs copy
    Action(const Action & action) = default;
    Action(Action && action)      = default;

    ActionType getType() const;
    void       setType(const ActionType type);

    QString getSrcKey() const;
    void    setSrcKey(const QString & key);

    QString getDstKey() const;
    void    setDstKey(const QString & key);

    const QString & getAppPath() const;
    void            setAppPath(const QString & path);

    const QString & getAppArgs() const;
    void            setAppArgs(const QString & path);

    const QString & getCreatedDate() const;

    void reset();

    Action & operator=(const Action & action) = default;
    Action & operator=(Action && action) = default;

private:
    ActionType m_Type;
    QString    m_SrcKey; // string with delimited ',' char. Could have multiple keys.
    QString    m_DstKey;
    QString    m_AppPath;
    QString    m_AppArgs;
    QString    m_CreatedDate;
};