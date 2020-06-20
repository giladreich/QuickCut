#include "pch.h"
#include "Profile.h"

Profile::Profile() noexcept
    : BaseModel()
    , m_bActive(false)
{
}

Profile::Profile(const std::string & id,
                 const std::string & name,
                 const std::string & lastModified,
                 bool                bActive) noexcept
    : BaseModel(id, name, lastModified)
    , m_bActive(bActive)
{
}

Profile::Profile(std::string && id,
                 std::string && name,
                 std::string && lastModified,
                 bool           bActive) noexcept
    : BaseModel(std::move(id), std::move(name), std::move(lastModified))
    , m_bActive(bActive)
{
}

Profile::~Profile()
{
    m_Actions.clear();
}

bool Profile::isActive() const
{
    return m_bActive;
}

void Profile::setActive(bool bActive)
{
    m_bActive = bActive;
}

void Profile::setActionsCapacity(int capacity)
{
    m_Actions.reserve(capacity);
}

const std::vector<Action *> & Profile::getActions() const
{
    return m_Actions;
}

int Profile::getActionsCount() const
{
    return (int)m_Actions.size();
}

bool Profile::addAction(Action * action)
{
    m_Actions.emplace_back(action);

    return true;
}

bool Profile::insertAction(uint32_t index, Action * action)
{
    m_Actions.insert(m_Actions.begin() + index + 1, action);

    return true;
}

bool Profile::deleteAction(Action * action)
{
    auto itr = std::find(m_Actions.begin(), m_Actions.end(), action);
    if (itr == m_Actions.end()) return false;

    delete action;
    m_Actions.erase(itr);

    return true;
}

bool Profile::moveActionDown(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= m_Actions.size() - 1) return false;

    std::swap(m_Actions[index], m_Actions[index + 1]);

    return true;
}

bool Profile::moveActionUp(int index)
{
    if (index < 1 || static_cast<size_t>(index) >= m_Actions.size()) return false;

    std::swap(m_Actions[index], m_Actions[index - 1]);

    return true;
}

bool Profile::deleteActionByIndex(const uint32_t & index)
{
    return deleteAction(getActionByIndex(index));
}

void Profile::getActionById(const std::string & uuid) const {}

void Profile::getActionByName(const std::string & name) const {}

Action * Profile::getActionByIndex(const uint32_t & index) const
{
    if (index > m_Actions.size()) return nullptr;

    return m_Actions[index];
}

Action * Profile::operator[](uint32_t index)
{
    if (index > m_Actions.size()) return nullptr;

    return m_Actions[index];
}