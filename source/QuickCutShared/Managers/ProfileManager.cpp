
#include "pch.h"
#include "ProfileManager.h"

ProfileManager::ProfileManager() noexcept
    : BaseManager()
    , m_Parser("Config/profiles.json")
{
}

ProfileManager::ProfileManager(const std::string & filePath) noexcept
    : BaseManager()
    , m_Parser(filePath)
{
}

ProfileManager::ProfileManager(std::string && filePath) noexcept
    : BaseManager()
    , m_Parser(std::move(filePath))
{
}

ProfileManager::~ProfileManager() = default;

bool ProfileManager::load()
{
    return m_Parser.parse(&m_vData);
}

bool ProfileManager::save()
{
    return m_Parser.save(m_vData);
}

const std::string & ProfileManager::getConfigFilePath()
{
    return m_Parser.getFilePath();
}

Profile * ProfileManager::getActiveProfile()
{
    auto activeProfile = std::find_if(m_vData.begin(), m_vData.end(),
                                      [](auto profile) { return profile->isActive(); });
    if (activeProfile == m_vData.end()) return nullptr;

    return *activeProfile;
}

void ProfileManager::setActiveProfile(int index)
{
    if (index < 0 || index >= count()) return;

    auto profile = m_vData[index];
    if (!profile) return;

    for (auto && profile : m_vData) profile->setActive(false);
    profile->setActive(true);
}

void ProfileManager::setActiveProfile(Profile * profile)
{
    if (!profile) return;

    auto itr = std::find(m_vData.begin(), m_vData.end(), profile);
    if (itr == m_vData.end()) return;

    for (auto && profile : m_vData) profile->setActive(false);
    (*itr)->setActive(true);
}
