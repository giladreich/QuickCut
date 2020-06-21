
#pragma once

#include "BaseModel.h"
#include "Managers/ActionManager.h"

class ActionManager;

class Profile : public BaseModel
{
public:
    // Constructs new profile
    Profile() noexcept;

    // Constructs existing profile
    Profile(const std::string & id,
            const std::string & name,
            const std::string & lastModified,
            bool                active = false) noexcept;

    Profile(std::string && id,
            std::string && name,
            std::string && lastModified,
            bool           active = false) noexcept;

    // Constructs copy
    Profile(const Profile & profile) = default;
    Profile(Profile && profile)      = default;

    ~Profile();

    bool isActive() const;
    void setActive(bool active);

    ActionManager & getActionManager();

    Profile & operator=(const Profile & profile) = default;
    Profile & operator=(Profile && profile) = default;

private:
    bool          m_Active;
    ActionManager m_Actions;
};
