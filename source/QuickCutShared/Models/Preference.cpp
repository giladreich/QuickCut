
#include "pch.h"
#include "Preference.h"

Preference::Preference()
    : m_ViewToolBar(true)
    , m_ViewStatusBar(true)
    , m_ThemeType(ThemeUbuntu)
{
}

QString Preference::getThemeFilePath() const
{
    return getThemeFilePath(m_ThemeType);
}

QString Preference::getThemeFilePath(ThemeType type)
{
    switch (type)
    {
        case ThemeDefault:
            return "";
        case ThemeDark:
            return ":stylesheet/theme_dark.qss";
        case ThemeBreezeDark:
            return ":stylesheet/theme_breezeDark.qss";
        case ThemeBreezeLight:
            return ":stylesheet/theme_breezeLight.qss";
        case ThemeConsoleDark:
            return ":stylesheet/theme_console.qss";
        case ThemeDarkOrange:
            return ":stylesheet/theme_darkorange.qss";
        case ThemeUbuntu:
        default:
            return ":stylesheet/theme_ubuntu.qss";
    }
}

bool Preference::isToolBarVisible() const
{
    return m_ViewToolBar;
}

void Preference::setToolBarVisible(bool visible)
{
    m_ViewToolBar = visible;
}

bool Preference::isStatusBarVisible() const
{
    return m_ViewStatusBar;
}

void Preference::setStatusBarVisible(bool visible)
{
    m_ViewStatusBar = visible;
}

ThemeType Preference::getThemeType() const
{
    return m_ThemeType;
}

void Preference::setThemeType(ThemeType type)
{
    m_ThemeType = type;
}
