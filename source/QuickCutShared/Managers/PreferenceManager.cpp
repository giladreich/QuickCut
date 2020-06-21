
#include "pch.h"
#include "PreferenceManager.h"

PreferenceManager::PreferenceManager() noexcept
    : m_Parser("Config/profiles.json")
{
}

PreferenceManager::PreferenceManager(const std::string & filePath) noexcept
    : m_Parser(filePath)
{
}

PreferenceManager::PreferenceManager(std::string && filePath) noexcept
    : m_Parser(std::move(filePath))
{
}

PreferenceManager::~PreferenceManager() = default;

bool PreferenceManager::load()
{
    return m_Parser.parse(&m_Preference);
}

bool PreferenceManager::save()
{
    return m_Parser.save(m_Preference);
}

const std::string & PreferenceManager::getConfigFilePath()
{
    return m_Parser.getFilePath();
}

Preference & PreferenceManager::get()
{
    return m_Preference;
}
