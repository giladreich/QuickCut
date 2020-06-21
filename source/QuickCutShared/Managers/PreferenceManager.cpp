
#include "pch.h"
#include "PreferenceManager.h"

PreferenceManager::PreferenceManager() noexcept
    : m_Parser("Config/profiles.json")
    , m_LoadSucceed(false)
{
}

PreferenceManager::PreferenceManager(const std::string & filePath) noexcept
    : m_Parser(filePath)
    , m_LoadSucceed(false)
{
}

PreferenceManager::PreferenceManager(std::string && filePath) noexcept
    : m_Parser(std::move(filePath))
    , m_LoadSucceed(false)
{
}

PreferenceManager::~PreferenceManager() = default;

bool PreferenceManager::load()
{
    m_LoadSucceed = m_Parser.parse(&m_Preference);
    return m_LoadSucceed;
}

bool PreferenceManager::save()
{
    return m_Parser.save(m_Preference);
}

bool PreferenceManager::isLoadSucceed()
{
    return m_LoadSucceed;
}

const std::string & PreferenceManager::getConfigFilePath()
{
    return m_Parser.getFilePath();
}

Preference & PreferenceManager::get()
{
    return m_Preference;
}
