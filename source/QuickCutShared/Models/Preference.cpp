
#include "pch.h"
#include "Preference.h"

Preference::Preference()
    : m_ToolBarVisible(true)
    , m_StatusBarVisible(true)
    , m_Theme()
{
}

bool Preference::isToolBarVisible() const
{
    return m_ToolBarVisible;
}

void Preference::setToolBarVisible(bool visible)
{
    m_ToolBarVisible = visible;
}

bool Preference::isStatusBarVisible() const
{
    return m_StatusBarVisible;
}

void Preference::setStatusBarVisible(bool visible)
{
    m_StatusBarVisible = visible;
}

QCTheme & Preference::getTheme()
{
    return m_Theme;
}

const QCTheme & Preference::theme() const
{
    return m_Theme;
}
