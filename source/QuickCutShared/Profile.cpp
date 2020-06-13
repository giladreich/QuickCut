
#include "Profile.h"
#include "pch.h"

Profile::Profile()
    : m_szUuid(createUuid())
    , m_szName("")
    , m_szLastModified(getDateTime())
{
}

Profile::Profile(std::string name)
    : m_szUuid(createUuid())
    , m_szName(std::move(name))
    , m_szLastModified(getDateTime())
{
}

Profile::Profile(std::string id, std::string name, std::string lastModified)
    : m_szUuid(std::move(id))
    , m_szName(std::move(name))
    , m_szLastModified(std::move(lastModified))
{
}

Profile::~Profile()
{
    for (auto itr : m_vActions)
    {
        Action * pAction = itr;
        delete pAction;
    }
    m_vActions.clear();
}

const std::string & Profile::getId() const
{
    return m_szUuid;
}

const std::string & Profile::getName() const
{
    return m_szName;
}

void Profile::setName(const std::string & name)
{
    m_szName = name;
    updated();
}

const std::string & Profile::getLastModified() const
{
    return m_szLastModified;
}

void Profile::setActionsCapacity(int capacity)
{
    m_vActions.reserve(capacity);
}

const std::vector<Action *> & Profile::getActions() const
{
    return m_vActions;
}

int Profile::getActionsCount() const
{
    return (int)m_vActions.size();
}

bool Profile::addAction(Action * action)
{
    m_vActions.emplace_back(action);
    updated();

    return true;
}

bool Profile::insertAction(uint32_t iIndex, Action * pAction)
{
    m_vActions.insert(m_vActions.begin() + iIndex + 1, pAction);
    updated();

    return true;
}

bool Profile::deleteAction(Action * pAction)
{
    auto itr = std::find(m_vActions.begin(), m_vActions.end(), pAction);
    if (itr == m_vActions.end()) return false;

    delete pAction;
    m_vActions.erase(itr);

    return true;
}

bool Profile::moveActionDown(int iIndex)
{
    if (iIndex < 0 || static_cast<size_t>(iIndex) >= m_vActions.size() - 1) return false;

    std::swap(m_vActions[iIndex], m_vActions[iIndex + 1]);

    return true;
}

bool Profile::moveActionUp(int iIndex)
{
    if (iIndex < 1 || static_cast<size_t>(iIndex) >= m_vActions.size()) return false;

    std::swap(m_vActions[iIndex], m_vActions[iIndex - 1]);

    return true;
}

bool Profile::deleteActionByIndex(const uint32_t & iIndex)
{
    return deleteAction(getActionByIndex(iIndex));
}

void Profile::getActionById(const std::string & uuid) const {}

void Profile::getActionByName(const std::string & name) const {}

Action * Profile::getActionByIndex(const uint32_t & iIndex) const
{
    if (iIndex > m_vActions.size()) return nullptr;

    return m_vActions[iIndex];
}

Action * Profile::operator[](uint32_t iIndex)
{
    if (iIndex > m_vActions.size()) return nullptr;

    return m_vActions[iIndex];
}

void Profile::updated()
{
    m_szLastModified = getDateTime();
}
