
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

class Preferences
{
public:
    Preferences();

    QString        getThemeFilePath() const;
    static QString getThemeFilePath(ThemeType type);
    ThemeType      getThemeType() const;
    void           setThemeType(ThemeType type);

public:
    bool m_ViewToolBar;
    bool m_ViewStatusBar;

    ThemeType m_ThemeType;
};