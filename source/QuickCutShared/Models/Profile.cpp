
#include "QuickCutShared/QuickCutPCH.h"
#include "Profile.h"

Profile::Profile() noexcept
    : BaseModel()
    , m_Active(false)
{
}

Profile::Profile(const QString & id,
                 const QString & name,
                 const QString & lastModified,
                 bool            active) noexcept
    : BaseModel(id, name, lastModified)
    , m_Active(active)
{
}

Profile::Profile(QString && id, QString && name, QString && lastModified, bool active) noexcept
    : BaseModel(std::move(id), std::move(name), std::move(lastModified))
    , m_Active(active)
{
}

Profile::Profile(const Profile & other)
{
    m_Active  = other.m_Active;
    m_Actions = ActionManager(other.m_Actions);
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
    m_Active       = active;
    m_LastModified = QuickCut::getDateTime();
}

ActionManager & Profile::getActionManager()
{
    return m_Actions;
}

const ActionManager & Profile::getActionManager() const
{
    return m_Actions;
}

Profile & Profile::operator=(const Profile & other)
{
    if (this != &other)
    {
        m_Active = other.m_Active;

        m_Actions.clear();
        m_Actions = ActionManager(other.m_Actions);
    }

    return *this;
}
