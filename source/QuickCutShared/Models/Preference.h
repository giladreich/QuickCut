
#pragma once

#include <QString>

enum ThemeType
{
    ThemeDefault = 0,
    ThemeDark,
    ThemeBreezeDark,
    ThemeBreezeLight,
    ThemeConsoleDark,
    ThemeUbuntu,
    ThemeDarkOrange,

    THEME_COUNT
};

class Preference
{
public:
    Preference();

    // Constructs copy
    Preference(const Preference & profile) = default;
    Preference(Preference && profile)      = default;

    bool isToolBarVisible() const;
    void setToolBarVisible(bool visible);

    bool isStatusBarVisible() const;
    void setStatusBarVisible(bool visible);

    ThemeType getCurrentTheme() const;
    void      setCurrentTheme(ThemeType type);

    QString        getCurrentThemeResourcePath() const;
    static QString getThemeResourcePath(ThemeType type);

    Preference & operator=(const Preference & profile) = default;
    Preference & operator=(Preference && profile) = default;

private:
    bool m_ToolBarVisible;
    bool m_StatusBarVisible;

    ThemeType m_CurrTheme;
};