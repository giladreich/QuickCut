
#pragma once

#include "BaseModel.h"
#include "Action.h"
#include <vector>

class Profile : public BaseModel
{
public:
    // Constructs new profile
    Profile() noexcept;

    // Constructs existing profile
    Profile(const std::string & id,
            const std::string & name,
            const std::string & lastModified,
            bool                bActive = false) noexcept;

    Profile(std::string && id,
            std::string && name,
            std::string && lastModified,
            bool           bActive = false) noexcept;

    // Constructs copy
    Profile(const Profile & profile) = default;
    Profile(Profile && profile)      = default;

    ~Profile();

    bool isActive() const;
    void setActive(bool bActive);

    void                          setActionsCapacity(int capacity);
    const std::vector<Action *> & getActions() const;
    int                           getActionsCount() const;

    bool addAction(Action * action);
    bool insertAction(uint32_t index, Action * action);
    bool deleteAction(Action * action);
    bool moveActionDown(int index);
    bool moveActionUp(int index);
    bool deleteActionByIndex(const uint32_t & index);

    void     getActionById(const std::string & uuid) const;
    void     getActionByName(const std::string & name) const;
    Action * getActionByIndex(const uint32_t & index) const;

    Action * operator[](uint32_t index);

    Profile & operator=(const Profile & profile) = default;
    Profile & operator=(Profile && profile) = default;

private:
    bool                  m_bActive;
    std::vector<Action *> m_Actions;
};
