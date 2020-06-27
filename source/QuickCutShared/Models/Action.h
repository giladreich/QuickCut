
#pragma once

#include "BaseModel.h"
#include "KeyData.h"

class Action : public BaseModel
{
    Q_GADGET
public:
    enum ActionType
    {
        ActionInvalid = -1,
        ActionKeyMap  = 0,
        ActionAppLaunch,
        ActionDirLaunch,

        ActionCount,
    };
    Q_ENUM(ActionType)

    // Constructs new Action
    Action() noexcept;

    // Constructs new Action
    Action(const QString &      name,
           const ActionType &   type,
           const KeyboardKeys & srcKeys,
           const KeyboardKeys & dstKeys,
           const QString &      targetPath,
           const QString &      appArgs,
           const bool &         enabled = true) noexcept;

    Action(QString &&      name,
           ActionType &&   type,
           KeyboardKeys && srcKeys,
           KeyboardKeys && dstKeys,
           QString &&      targetPath,
           QString &&      appArgs,
           bool &&         enabled = true) noexcept;

    // Constructs existing Action
    Action(const QString &      id,
           const QString &      name,
           const QString &      lastModified,
           const ActionType &   type,
           const KeyboardKeys & srcKeys,
           const KeyboardKeys & dstKeys,
           const QString &      targetPath,
           const QString &      appArgs,
           const QString &      createdDate,
           const bool &         enabled) noexcept;

    Action(QString &&      id,
           QString &&      name,
           QString &&      lastModified,
           ActionType &&   type,
           KeyboardKeys && srcKeys,
           KeyboardKeys && dstKeys,
           QString &&      targetPath,
           QString &&      appArgs,
           QString &&      createdDate,
           bool &&         enabled) noexcept;

    // Constructs copy
    Action(const Action & action) = default;
    Action(Action && action)      = default;

    ActionType getType() const;
    void       setType(const ActionType type);

    const QString & getTargetPath() const;
    void            setTargetPath(const QString & path);

    const QString & getAppArgs() const;
    void            setAppArgs(const QString & path);

    const QString & getCreatedDate() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    void reset();

public: // Keys operations
    const KeyboardKeys & getSrcKeys() const;
    void                 setSrcKeys(const KeyboardKeys & keys);
    QStringList          getSrcKeysNames() const;
    QString              getSrcKeysName(QChar delimiter = '+') const;
    QString              getSrcKeysCode() const;

    const KeyboardKeys & getDstKeys() const;
    void                 setDstKeys(const KeyboardKeys & keys);
    QStringList          getDstKeysNames() const;
    QString              getDstKeysName(QChar delimiter = '+') const;
    QString              getDstKeysCode() const;

    static QStringList getKeysNames(const KeyboardKeys & keys);
    static QString     getKeysCode(const KeyboardKeys & keys);

public: // Operators
    Action & operator=(const Action & action) = default;
    Action & operator=(Action && action) = default;

private:
    ActionType   m_Type;
    KeyboardKeys m_SrcKeys;
    KeyboardKeys m_DstKeys;
    QString      m_TargetPath;
    QString      m_AppArgs;
    QString      m_CreatedDate;
    bool         m_Enabled;
};