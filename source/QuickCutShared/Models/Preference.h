
#pragma once

#include "QCTheme.h"

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

    QCTheme &       getTheme();
    const QCTheme & theme() const;

    Preference & operator=(const Preference & profile) = default;
    Preference & operator=(Preference && profile) = default;

private:
    bool m_ToolBarVisible;
    bool m_StatusBarVisible;

    QCTheme m_Theme;
};
