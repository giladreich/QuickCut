
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

// TODO(Gilad): Rename to singular.
class Preferences
{
public:
    Preferences();

    // Constructs copy
    Preferences(const Preferences & profile) = default;
    Preferences(Preferences && profile)      = default;

    QString        getThemeFilePath() const;
    static QString getThemeFilePath(ThemeType type);

    bool isToolBarVisible() const;
    void setToolBarVisible(bool visible);

    bool isStatusBarVisible() const;
    void setStatusBarVisible(bool visible);

    ThemeType getThemeType() const;         // TODO(Gilad): Rename to getCurrentTheme
    void      setThemeType(ThemeType type); // TODO(Gilad): Rename to getCurrentTheme

    Preferences & operator=(const Preferences & profile) = default;
    Preferences & operator=(Preferences && profile) = default;

public: // TODO(Gilad): Make it private.
    bool m_ViewToolBar;   // TODO(Gilad): Rename to m_ToolBarVisible
    bool m_ViewStatusBar; // TODO(Gilad): Rename to m_StatusBarVisible

    ThemeType m_ThemeType; // TODO(Gilad): Rename to m_CurrTheme
};