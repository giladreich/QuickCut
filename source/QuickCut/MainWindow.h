
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "Models/Profile.h"
#include "Preferences.h"

class ExamplesWindow;
class CheckUpdatesWindow;
class AboutWindow;
class ActionEditWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

    void connectSlots();

    void activateHook();
    void initPreferences();
    void initProfiles();

    // TODO: Implement parser and move all of these.
    bool loadPreferences(const std::string & path, Preferences * outPreferences);
    bool savePreferences(const std::string & path, const Preferences & preferences);
    bool loadPreferences();
    bool savePreferences();

    bool loadProfiles(const std::string & profilesPath, std::string * activeProfileOut);
    bool loadProfiles(const QString & profilesPath, QString * activeProfileOut);
    bool loadProfiles(const QString & profilesPath, std::string * activeProfileOut);
    bool reloadProfiles();
    bool saveProfiles(const std::string & profilesPath, const std::string & activeProfileId);
    bool saveProfiles(const QString & profilesPath, const QString & activeProfileId);
    bool saveProfiles();

    bool isActiveProfile(Profile * profile);
    void showEvent(QShowEvent * event) override;

public slots:

    void onProfileSelChange(int index);
    void onBtnSetActiveProfile();
    void onBtnDeleteProfile();
    void onBtnCreateProfile();

    void onActionSelChange(int index);
    void onActionDoubleClicked(const QModelIndex & index);
    void onBtnActionCreate();
    void onBtnActionDelete();
    void onBtnActionDuplicate();
    void onBtnActionMoveDown();
    void onBtnActionMoveUp();

    void listItemSwap(QListWidget * list, bool moveUp);

    void onActionSaved();
    void onActionCreated(Action * action);

    // File Menu
    void onActionFileOpen();
    void onActionFileSave();
    void onActionFileSaveAs();
    void onActionFileRestartService();
    void onActionFileExit();

    void onActionViewToolBar();
    void onActionViewStatusBar();
    void onActionViewRefresh();

    void onActionHelpAbout();
    void onActionHelpExamples();
    void onActionHelpCheckUpdates();

    void onActionLoadTheme(eThemeType type, QAction * action = nullptr);
    void onActionLoadTheme(const QString & qssPath, QAction * action = nullptr);
    void onLoadCustomStylesheet();

private:
    Ui::MainWindowClass * ui;

    ActionEditWindow *   m_ActionEditWindow;
    AboutWindow *        m_AboutWindow;
    CheckUpdatesWindow * m_CheckUpdatesWindow;
    ExamplesWindow *     m_ExamplesWindow;

    QMap<eThemeType, QAction *> m_ThemeActions;
    QList<Profile *>            m_Profiles;
    QString                     m_ProfilesPath;
    QString                     m_ActiveProfile;
    Preferences                 m_Preferences;
};
