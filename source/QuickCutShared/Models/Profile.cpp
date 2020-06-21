
#include "pch.h"
#include "Profile.h"

Profile::Profile() noexcept
    : BaseModel()
    , m_Active(false)
{
}

Profile::Profile(const std::string & id,
                 const std::string & name,
                 const std::string & lastModified,
                 bool                active) noexcept
    : BaseModel(id, name, lastModified)
    , m_Active(active)
{
}

Profile::Profile(std::string && id,
                 std::string && name,
                 std::string && lastModified,
                 bool           active) noexcept
    : BaseModel(std::move(id), std::move(name), std::move(lastModified))
    , m_Active(active)
{
}

Profile::~Profile()
{
    m_Actions.clear();
}

bool Profile::isActive() const
{
    return m_Active;
}

void Profile::setActive(bool active)
{
    m_Active = active;
}

ActionManager & Profile::getActionManager()
{
    return m_Actions;
}
