
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "Profile.h"
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
    bool loadPreferences(const std::string & szPath, Preferences * pOutPreferences);
    bool savePreferences(const std::string & szPath, const Preferences & preferences);
    bool loadPreferences();
    bool savePreferences();

    bool loadProfiles(const std::string & szProfilesPath, std::string * pszActiveProfileOut);
    bool loadProfiles(const QString & szProfilesPath, QString * pszActiveProfileOut);
    bool loadProfiles(const QString & szProfilesPath, std::string * pszActiveProfileOut);
    bool reloadProfiles();
    bool saveProfiles(const std::string & szProfilesPath, const std::string & szActiveProfileId);
    bool saveProfiles(const QString & szProfilesPath, const QString & szActiveProfileId);
    bool saveProfiles();


    bool isActiveProfile(Profile * pProfile);
    void showEvent(QShowEvent * event) override;


public slots:

    void onProfileSelChange(int iIndex);
    void onBtnSetActiveProfile();
    void onBtnDeleteProfile();
    void onBtnCreateProfile();

    void onActionSelChange(int iIndex);
    void onActionDoubleClicked(const QModelIndex & index);
    void onBtnActionCreate();
    void onBtnActionDelete();
    void onBtnActionDuplicate();
    void onBtnActionMoveDown();
    void onBtnActionMoveUp();

    void listItemSwap(QListWidget * pList, bool bMoveUp);

    void onActionSaved();
    void onActionCreated(Action * pAction);

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

    void onActionLoadTheme(eThemeType eType, QAction * pAction = nullptr);
    void onActionLoadTheme(const QString & szQssPath, QAction * pAction = nullptr);
    void onLoadCustomStylesheet();


private:
    Ui::MainWindowClass * ui;

    ActionEditWindow *           m_pActionEditWindow;
    AboutWindow *                m_pAboutWindow;
    CheckUpdatesWindow *         m_pCheckUpdatesWindow;
    ExamplesWindow *             m_pExamplesWindow;
                           
    QMap<eThemeType, QAction *>  m_qThemeActions;
    QList<Profile *>             m_qProfiles;
    QString                      m_qszProfilesPath;
    QString                      m_qszActiveProfile;
    Preferences                  m_Preferences;

};
