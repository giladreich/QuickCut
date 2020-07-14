
#pragma once

#include "BaseModel.h"

#include "QuickCutShared/Managers/ActionManager.h"

class ActionManager;

class Profile : public BaseModel
{
public:
    // Constructs new profile
    Profile() noexcept;
    Profile(const QString & id,
            const QString & lastModified,
            const QString & createdDate) noexcept;
    Profile(QString && id, QString && lastModified, QString && createdDate) noexcept;

    // Constructs copy
    Profile(const Profile & other);
    Profile(Profile && other) = default;

    ~Profile();

    bool isActive() const;
    void setActive(bool active);

    ActionManager &       getActionManager();
    const ActionManager & getActionManager() const;

    Profile & operator=(const Profile & other);
    Profile & operator=(Profile && other) = default;

private:
    bool          m_Active;
    ActionManager m_Actions;
};
