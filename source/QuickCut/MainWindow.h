
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "Managers/ProfileManager.h"
#include "Managers/PreferenceManager.h"

class ExamplesWindow;
class CheckUpdatesWindow;
class AboutWindow;
class ActionEditWindow;
class QLocalSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

    void connectSlots();

    void initThemes();
    void initPreference();
    void initProfiles();
    void activateHook();

    bool loadProfiles();
    bool reloadProfiles();
    bool saveProfiles();

    bool sendReloadProfiles();

    void showEvent(QShowEvent * event) override;

public slots:

    void      onProfileSelChange(int index);
    void      onBtnSetActiveProfile();
    void      onBtnDeleteProfile();
    Profile * onBtnCreateProfile();

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

    void onReloadProfilesResponse();

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

    void onActionLoadTheme(QCTheme::ThemeType type);
    void onActionLoadTheme(const QString & qssPath, QAction * action = nullptr);
    void onLoadCustomStylesheet();

private:
    Ui::MainWindowClass * ui;

    QPointer<ActionEditWindow>   m_ActionEditWindow;
    QPointer<AboutWindow>        m_AboutWindow;
    QPointer<CheckUpdatesWindow> m_CheckUpdatesWindow;
    QPointer<ExamplesWindow>     m_ExamplesWindow;

    QMap<QCTheme::ThemeType, QAction *> m_ThemeActions;

    ProfileManager    m_Profiles;
    PreferenceManager m_Preference;

    QLocalSocket * m_LocalSocket;
    QDataStream    m_SocketStreamIn;
    int            m_SocketBlockSize;
};
