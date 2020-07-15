
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
        ActionFileLaunch,
        ActionDirLaunch,
        ActionAutoText,

        ActionCount,
    };
    Q_ENUM(ActionType)

    // Constructs new Action
    Action() noexcept;

    // Constructs existing Action
    Action(const QString & id,
           const QString & lastModified,
           const QString & createdDate) noexcept;
    Action(QString && id, QString && lastModified, QString && createdDate) noexcept;

    // Constructs copy
    Action(const Action & action) = default;
    Action(Action && action)      = default;

    ActionType     getType() const;
    QString        getTypeName() const;
    static QString getTypeName(ActionType type);
    void           setType(const ActionType type);

    const QString & getTargetPath() const;
    void            setTargetPath(const QString & path);

    const QString & getTargetArgs() const;
    void            setTargetArgs(const QString & path);

    const QString & getAutoText() const;
    void            setAutoText(const QString & path);

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
    QString      m_TargetPath;
    QString      m_TargetArgs;
    QString      m_AutoText;
    bool         m_Enabled;
    KeyboardKeys m_SrcKeys;
    KeyboardKeys m_DstKeys;
};
