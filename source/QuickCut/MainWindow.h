
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "Managers/ProfileManager.h"
#include "Managers/PreferenceManager.h"

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

    void initPreference();
    void initProfiles();
    void activateHook();

    bool loadProfiles();
    bool reloadProfiles();
    bool saveProfiles();

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

    void onActionLoadTheme(ThemeType type, QAction * action = nullptr);
    void onActionLoadTheme(const QString & qssPath, QAction * action = nullptr);
    void onLoadCustomStylesheet();

private:
    Ui::MainWindowClass * ui;

    ActionEditWindow *   m_ActionEditWindow;
    AboutWindow *        m_AboutWindow;
    CheckUpdatesWindow * m_CheckUpdatesWindow;
    ExamplesWindow *     m_ExamplesWindow;

    QMap<ThemeType, QAction *> m_ThemeActions;

    ProfileManager    m_Profiles;
    PreferenceManager m_Preference;
};
