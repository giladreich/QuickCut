
#include "Profile.h"
#include "pch.h"

Profile::Profile()
    : m_Uuid(createUuid())
    , m_Name("")
    , m_LastModified(getDateTime())
{
}

Profile::Profile(std::string name)
    : m_Uuid(createUuid())
    , m_Name(std::move(name))
    , m_LastModified(getDateTime())
{
}

Profile::Profile(std::string id, std::string name, std::string lastModified)
    : m_Uuid(std::move(id))
    , m_Name(std::move(name))
    , m_LastModified(std::move(lastModified))
{
}

Profile::~Profile()
{
    for (auto itr : m_Actions)
    {
        Action * action = itr;
        delete action;
    }
    m_Actions.clear();
}

const std::string & Profile::getId() const
{
    return m_Uuid;
}

const std::string & Profile::getName() const
{
    return m_Name;
}

void Profile::setName(const std::string & name)
{
    m_Name = name;
    updated();
}

const std::string & Profile::getLastModified() const
{
    return m_LastModified;
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
    updated();

    return true;
}

bool Profile::insertAction(uint32_t index, Action * action)
{
    m_Actions.insert(m_Actions.begin() + index + 1, action);
    updated();

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

void Profile::updated()
{
    m_LastModified = getDateTime();
}
