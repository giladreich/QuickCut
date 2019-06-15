
#include "MainWindow.h"
#include "pch.h"

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QStandardPaths>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QtService/QtService>

#include "ActionEditWindow.h"
#include "AboutWindow.h"
#include "CheckUpdatesWindow.h"
#include "ExamplesWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass)
    , m_pActionEditWindow(nullptr)
    , m_pAboutWindow(nullptr)
    , m_pCheckUpdatesWindow(nullptr)
    , m_pExamplesWindow(nullptr)
{
    ui->setupUi(this);

    connectSlots();

    initPreferences();
    initProfiles();
    activateHook();

    statusBar()->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_qProfiles);
    m_qProfiles.clear();
}

void MainWindow::connectSlots()
{
    // File Menu
    connect(ui->actionFileOpen,           &QAction::triggered, this, &MainWindow::onActionFileOpen);
    connect(ui->actionFileSave,           &QAction::triggered, this, &MainWindow::onActionFileSave);
    connect(ui->actionFileSaveAs,         &QAction::triggered, this, &MainWindow::onActionFileSaveAs);
    connect(ui->actionFileRestartService, &QAction::triggered, this, &MainWindow::onActionFileRestartService);
    connect(ui->actionFileExit,           &QAction::triggered, this, &MainWindow::onActionFileExit);

    // View Menu
    connect(ui->actionViewToolBar,   &QAction::triggered, this, &MainWindow::onActionViewToolBar);
    connect(ui->actionViewStatusBar, &QAction::triggered, this, &MainWindow::onActionViewStatusBar);
    connect(ui->actionViewRefresh,   &QAction::triggered, this, &MainWindow::onActionViewRefresh);

    // Preferences Menu
    connect(ui->actionThemeDefault,     &QAction::triggered, this, [this] { onActionLoadTheme(ThemeDefault, ui->actionThemeDefault); });
    connect(ui->actionThemeDark,        &QAction::triggered, this, [this] { onActionLoadTheme(ThemeDark, ui->actionThemeDark); });
    connect(ui->actionThemeBreezeDark,  &QAction::triggered, this, [this] { onActionLoadTheme(ThemeBreezeDark, ui->actionThemeBreezeDark); });
    connect(ui->actionThemeBreezeLight, &QAction::triggered, this, [this] { onActionLoadTheme(ThemeBreezeLight, ui->actionThemeBreezeLight); });
    connect(ui->actionThemeConsoleDark, &QAction::triggered, this, [this] { onActionLoadTheme(ThemeConsoleDark, ui->actionThemeConsoleDark); });
    connect(ui->actionThemeUbuntu,      &QAction::triggered, this, [this] { onActionLoadTheme(ThemeUbuntu, ui->actionThemeUbuntu); });
    m_qThemeActions.insert(ThemeDefault    , ui->actionThemeDefault);
    m_qThemeActions.insert(ThemeDark       , ui->actionThemeDark);
    m_qThemeActions.insert(ThemeBreezeDark , ui->actionThemeBreezeDark);
    m_qThemeActions.insert(ThemeBreezeLight, ui->actionThemeBreezeLight);
    m_qThemeActions.insert(ThemeConsoleDark, ui->actionThemeConsoleDark);
    m_qThemeActions.insert(ThemeUbuntu     , ui->actionThemeUbuntu);

    connect(ui->actionThemeLoadQss,     &QAction::triggered, this, &MainWindow::onLoadCustomStylesheet);

    // Help Menu
    connect(ui->actionHelpAbout,        &QAction::triggered, this, &MainWindow::onActionHelpAbout);
    connect(ui->actionHelpExamples,     &QAction::triggered, this, &MainWindow::onActionHelpExamples);
    connect(ui->actionHelpCheckUpdates, &QAction::triggered, this, &MainWindow::onActionHelpCheckUpdates);


    // Controls
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this, &MainWindow::onActionSelChange);
    connect(ui->lbxActions, &QListWidget::doubleClicked, this, &MainWindow::onActionDoubleClicked);
    connect(ui->cbxProfile, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onProfileSelChange);

    connect(ui->btnSetActive,       &QPushButton::clicked, this, &MainWindow::onBtnSetActiveProfile);
    connect(ui->btnCreateProfile,   &QPushButton::clicked, this, &MainWindow::onBtnCreateProfile);
    connect(ui->btnDeleteProfile,   &QPushButton::clicked, this, &MainWindow::onBtnDeleteProfile);

    connect(ui->btnActionCreate,    &QPushButton::clicked, this, &MainWindow::onBtnActionCreate);
    connect(ui->btnActionDelete,    &QPushButton::clicked, this, &MainWindow::onBtnActionDelete);
    connect(ui->btnActionDuplicate, &QPushButton::clicked, this, &MainWindow::onBtnActionDuplicate);
}

void MainWindow::activateHook()
{
    QProcess::execute("QuickCutService -i"); // TODO: Configure from installer.
    QtServiceController service("QuickCut Service");
    if (!service.isRunning())
    {
        service.start();
        ui->statusBar->showMessage("Activated QuickCut Service.");
    }

#ifdef Q_OS_WIN
    // TODO: Configure from installer.
    // Don't start the service right when win boots.
    QProcess::execute("sc config \"QuickCut Service\" start=delayed-auto");
    QProcess::execute("sc failure \"QuickCut Service\" actions=restart/60000/restart/60000/""/60000 reset=86400");

    const QString qcc = QCoreApplication::applicationDirPath() + "/QuickCutConsole.exe";

    // Approach 1:
    //QProcess::execute("schtasks /Create /TN \"QuickCut\" /SC ONLOGON /TR \"" + qcc + "\"");

    // Approach 2:
    //QSettings settings(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", QSettings::NativeFormat);
    //const QString value(settings.value("QuickCutConsole", QString()).toString());
    //if (value.isEmpty() || value != qcc)
    //    settings.setValue("QuickCutConsole", qcc);

    // Approach 3: <- Simple and works.
    QDir dir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
    dir.cdUp();
    QFile::link(qcc, dir.path() + "/Microsoft/Windows/Start Menu/Programs/Startup/QuickCutConsole.lnk");

    // Windows service starts the console hook as the default user using the winsta0\\default 
    // which sometimes fails(QuickCutServiceWindows::RunProcessAsUserW), so if the user starts QuickCut
    // GUI, we use the chance to start QuickCutConsole as the current user just in case.
    QTimer::singleShot(5000, [] {
        QThread * t = QThread::create([] {
            QProcess::execute("taskkill /im QuickCutConsole.exe /f");
            QThread::msleep(500);
            QProcess::execute("QuickCutConsole");
        });
        t->start();
    });
#endif
}


void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::initPreferences()
{
    QFileInfo fiPre("Config/preferences.json");
    if (!fiPre.exists())
    {
        if (!fiPre.dir().exists())
            fiPre.dir().mkdir(".");

        savePreferences(fiPre.filePath().toStdString(), m_Preferences);
    }
    else
    {
        loadPreferences(fiPre.filePath().toStdString(), &m_Preferences);
    }

    QAction * pAction = nullptr;
    switch (m_Preferences.getThemeType())
    {
    case ThemeDefault:     pAction = ui->actionThemeDefault;     break;
    case ThemeDark:        pAction = ui->actionThemeDark;        break;
    case ThemeBreezeDark:  pAction = ui->actionThemeBreezeDark;  break;
    case ThemeBreezeLight: pAction = ui->actionThemeBreezeLight; break;
    case ThemeConsoleDark: pAction = ui->actionThemeConsoleDark; break;;
    case ThemeUbuntu:      pAction = ui->actionThemeUbuntu;      break;
    default:
        break;
    }

    onActionLoadTheme(m_Preferences.getThemeFilePath(), pAction);

    ui->actionViewToolBar->setChecked(m_Preferences.m_bViewToolBar);
    ui->actionViewStatusBar->setChecked(m_Preferences.m_bViewStatusBar);
    ui->toolBar->setVisible(m_Preferences.m_bViewToolBar);
    ui->statusBar->setVisible(m_Preferences.m_bViewStatusBar);
}

void MainWindow::initProfiles()
{
    ui->cbxProfile->clear();
    QFileInfo fiProfiles("Config/profiles.json");
    if (!fiProfiles.exists())
    {
        if (!fiProfiles.dir().exists())
            fiProfiles.dir().mkdir(".");

        m_qszProfilesPath = fiProfiles.filePath();
        onBtnCreateProfile();
        // First created profile, need to set as default.
        Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
        m_qszActiveProfile = QString::fromStdString(pProfile->getId());
        saveProfiles();
        ui->btnSetActive->setEnabled(false);
    }
    else
    {
        m_qszProfilesPath = fiProfiles.filePath();

        String szActiveProfile;
        loadProfiles(m_qszProfilesPath, &szActiveProfile);
        m_qszActiveProfile = QString::fromStdString(szActiveProfile);

        for (auto && pProfile : m_qProfiles)
        {
            QString szProfileName = QString::fromStdString(pProfile->getName());
            ui->cbxProfile->addItem(szProfileName);

            if (szActiveProfile != pProfile->getId()) continue; // Not active profile

            ui->lbxActions->clear();
            for (auto && pAction : pProfile->getActions())
                ui->lbxActions->addItem(QString::fromStdString(pAction->getName()));

            ui->cbxProfile->setCurrentText(szProfileName);
        }

        ui->lbxActions->setCurrentRow(0);
    }
}

#pragma region --- Parser ---
bool MainWindow::loadPreferences(const std::string & szPath, Preferences * pOutPreferences)
{
    if (szPath.empty() || !pOutPreferences)
        return false;

    JSON rootJson;
    bpt::read_json(szPath, rootJson);

    int iTheme = rootJson.get<int>("uiThemeType", static_cast<int>(ThemeUbuntu));
    pOutPreferences->setThemeType(static_cast<eThemeType>(iTheme));
    pOutPreferences->m_bViewToolBar = rootJson.get<bool>("viewToolBar", true);
    pOutPreferences->m_bViewStatusBar = rootJson.get<bool>("viewStatusBar", true);

    return true;
}

bool MainWindow::savePreferences(const std::string & szPath, const Preferences & preferences)
{
    if (szPath.empty())
        return false;

    JSON rootJson;
    rootJson.put("uiThemeType", preferences.getThemeType());
    rootJson.put("viewToolBar", preferences.m_bViewToolBar);
    rootJson.put("viewStatusBar", preferences.m_bViewStatusBar);
    bpt::write_jsonEx(szPath, rootJson);

    return true;
}

bool MainWindow::loadPreferences()
{
    return loadPreferences("Config/preferences.json", &m_Preferences);
}

bool MainWindow::savePreferences()
{
    return savePreferences("Config/preferences.json", m_Preferences);
}

bool MainWindow::loadProfiles(const std::string & szProfilesPath, std::string * pszActiveProfileOut)
{
    if (szProfilesPath.empty())
        return false;

    JSON rootJson;
    bpt::read_json(szProfilesPath, rootJson);
    *pszActiveProfileOut = rootJson.get<String>("activeProfile", "");
    int profileCount  = rootJson.get<int>("profileCount", 0);
    qDeleteAll(m_qProfiles);
    m_qProfiles.clear();
    m_qProfiles.reserve(profileCount);

    JSON profilesJson = rootJson.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        String profileId    = profileJson.second.get<String>("id", "");
        String profileName  = profileJson.second.get<String>("name", "");
        String lastModified = profileJson.second.get<String>("lastModified", "");
        int actionsCount    = profileJson.second.get<int>("actionsCount", 0);

        Profile * pProfile = new Profile(profileId, profileName, lastModified);
        pProfile->setActionsCapacity(actionsCount);

        JSON actionsJson = profileJson.second.get_child("actions");
        for (auto && actionJson : actionsJson)
        {
            String actionId    = actionJson.second.get<String>("id", "");
            String actionName  = actionJson.second.get<String>("actionName", "");
            String actionType  = actionJson.second.get<String>("type", "");
            String srcKey      = actionJson.second.get<String>("srcKey", "");
            String dstKey      = actionJson.second.get<String>("dstKey", "");
            String appPath     = actionJson.second.get<String>("appPath", "");
            String appArgs     = actionJson.second.get<String>("appArgs", "");
            String createdDate = actionJson.second.get<String>("createdDate", "");

            pProfile->addAction(new Action(actionId, actionName, Action::getType(actionType),
                                           srcKey, dstKey,
                                           appPath, appArgs, createdDate));
        }

        m_qProfiles.push_back(pProfile);
    }

    return true;
}

bool MainWindow::loadProfiles(const QString & szProfilesPath, QString * pszActiveProfileOut)
{
    String szActiveProfile;
    if (!loadProfiles(szProfilesPath.toStdString(), &szActiveProfile))
        return false;

    *pszActiveProfileOut = QString::fromStdString(szActiveProfile);

    return true;
}

bool MainWindow::loadProfiles(const QString & szProfilesPath, std::string * pszActiveProfileOut)
{
    return loadProfiles(szProfilesPath.toStdString(), pszActiveProfileOut);
}

bool MainWindow::reloadProfiles()
{
    if (!loadProfiles(m_qszProfilesPath, &m_qszActiveProfile))
        return false;

    onProfileSelChange(ui->cbxProfile->currentIndex());

    return true;
}

bool MainWindow::saveProfiles(const std::string & szProfilesPath, const std::string & szActiveProfileId)
{
    if (szProfilesPath.empty() || szActiveProfileId.empty())
        return false;

    JSON rootJson;
    rootJson.put("activeProfile", szActiveProfileId);
    rootJson.put("profileCount", m_qProfiles.size());

    JSON profilesJson;
    for (auto && pProfile : m_qProfiles)
    {
        JSON profileJson;
        profileJson.put("id", pProfile->getId());
        profileJson.put("name", pProfile->getName());
        profileJson.put("lastModified", pProfile->getLastModified());
        profileJson.put("actionsCount", pProfile->getActionsCount());

        JSON actionsJson;
        for (auto && pAction : pProfile->getActions())
        {
            JSON actionJson;
            actionJson.put("id", pAction->getId());
            actionJson.put("actionName", pAction->getName());
            actionJson.put("type", Action::getType(pAction->getType()));
            actionJson.put("srcKey", pAction->getSrcKey());
            actionJson.put("dstKey", pAction->getDstKey());
            actionJson.put("appPath", pAction->getAppPath());
            actionJson.put("appArgs", pAction->getAppArgs());
            actionJson.put("createdDate", pAction->getCreatedDate());

            actionsJson.push_back(std::make_pair("", actionJson));
        }

        profileJson.push_back(std::make_pair("actions", actionsJson));
        profilesJson.push_back(std::make_pair("", profileJson));
    }
    rootJson.add_child("profiles", profilesJson);

    bpt::write_jsonEx(szProfilesPath, rootJson);
    Hook::sendReloadSignal();

    return true;
}

bool MainWindow::saveProfiles(const QString & szProfilesPath, const QString & szActiveProfileId)
{
    return saveProfiles(szProfilesPath.toStdString(), szActiveProfileId.toStdString());
}

bool MainWindow::saveProfiles()
{
    return saveProfiles(m_qszProfilesPath, m_qszActiveProfile);
}

// end  --- Parser ---
#pragma endregion


bool MainWindow::isActiveProfile(Profile * pProfile)
{
    return pProfile->getId() == m_qszActiveProfile.toStdString();
}

void MainWindow::onProfileSelChange(int iIndex)
{
    if (iIndex < 0 || iIndex > m_qProfiles.size())
    {
        ui->lbxActions->clear();
        ui->btnSetActive->setEnabled(false);
        ui->btnDeleteProfile->setEnabled(false);
        ui->btnActionCreate->setEnabled(false);
        return;
    }

    Profile * pProfile = m_qProfiles[iIndex];
    ui->btnSetActive->setEnabled(!isActiveProfile(pProfile));
    ui->btnDeleteProfile->setEnabled(true);
    ui->btnActionCreate->setEnabled(true);

    disconnect(ui->lbxActions, &QListWidget::currentRowChanged, this, &MainWindow::onActionSelChange);
    ui->lbxActions->clear();
    for (auto && pAction : pProfile->getActions())
        ui->lbxActions->addItem(QString::fromStdString(pAction->getName()));
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this, &MainWindow::onActionSelChange);
    
    if (!pProfile->getActionsCount())
        onActionSelChange(-1);
    else
        ui->lbxActions->setCurrentRow(0);
}

void MainWindow::onBtnSetActiveProfile()
{
    Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
    JSON rootJson;
    bpt::read_json(m_qszProfilesPath.toStdString(), rootJson);
    rootJson.put("activeProfile", pProfile->getId());
    bpt::write_jsonEx(m_qszProfilesPath.toStdString(), rootJson);
    reloadProfiles();
    ui->btnSetActive->setEnabled(false);
}

void MainWindow::onBtnDeleteProfile()
{
    const int iIndex = ui->cbxProfile->currentIndex();
    Profile * pProfile = m_qProfiles[iIndex];
    ui->cbxProfile->removeItem(iIndex);
    if (isActiveProfile(pProfile) && m_qProfiles.count() > 0)
    {
        m_qszActiveProfile = QString::fromStdString(m_qProfiles.first()->getId());
        delete pProfile;
        ui->cbxProfile->setCurrentIndex(0);
    }

    m_qProfiles.removeAt(iIndex);

    saveProfiles();
    reloadProfiles();
}

void MainWindow::onBtnCreateProfile()
{
    bool bOk;
    QString szProfileName = QInputDialog::getText(this, tr("Create Profile"),
                                                  tr("Profile Name:"), QLineEdit::Normal,
                                                  QDir::home().dirName(), &bOk);
    if (!bOk && szProfileName.isEmpty()) return;

    const int iIndex = ui->cbxProfile->count() > 0 ? ui->cbxProfile->currentIndex() + 1 : 0;

    Profile * pProfile = new Profile(szProfileName.toStdString());
    m_qProfiles.push_back(pProfile);
    ui->cbxProfile->addItem(szProfileName);
    ui->cbxProfile->setCurrentIndex(iIndex);
    saveProfiles();
}

void MainWindow::onActionSelChange(int iIndex)
{
    const bool bEnabled = iIndex != -1;
    ui->btnActionDelete->setEnabled(bEnabled);
    ui->btnActionDuplicate->setEnabled(bEnabled);
}

void MainWindow::onActionDoubleClicked(const QModelIndex & index)
{
    Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
    Action * pAction = pProfile->getActionByIndex(index.row());
    m_pActionEditWindow = new ActionEditWindow(this, pAction);
    connect(m_pActionEditWindow, &ActionEditWindow::onSaved, this, &MainWindow::onActionSaved);
    m_pActionEditWindow->exec();
}

void MainWindow::onBtnActionCreate()
{
    m_pActionEditWindow = new ActionEditWindow(this);
    connect(m_pActionEditWindow, &ActionEditWindow::onCreated, this, &MainWindow::onActionCreated);
    m_pActionEditWindow->exec();
}

void MainWindow::onBtnActionDelete()
{
    const int iIndex = ui->lbxActions->currentRow();
    Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
    pProfile->deleteActionByIndex(iIndex);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(iIndex - 1);
    if ((iIndex - 1) < 0)
        ui->btnActionCreate->setFocus();
}

void MainWindow::onBtnActionDuplicate()
{
    const int iIndex = ui->lbxActions->currentRow();
    Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
    Action * pAction = pProfile->getActionByIndex(iIndex);

    Action * pNewAction = new Action(pAction->getName(), pAction->getType(), pAction->getSrcKey(),
                                     pAction->getDstKey(), pAction->getAppPath(), pAction->getAppArgs());

    pProfile->insertAction(iIndex, pNewAction);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(iIndex + 1);
}

void MainWindow::onActionSaved()
{
    const int iIndex = ui->lbxActions->currentRow();
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(iIndex);
}

void MainWindow::onActionCreated(Action * pAction)
{
    const int iIndex = ui->lbxActions->currentRow();
    Profile * pProfile = m_qProfiles[ui->cbxProfile->currentIndex()];
    pProfile->addAction(pAction);
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(iIndex == -1 ? 0 : iIndex + 1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::onActionFileOpen()
{
    QString szDesktop = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();
    QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Profiles File"), szDesktop, tr("Profiles File (*.json)"));
    if (szFileName.isEmpty()) return;

    auto answer = QMessageBox::warning(this, "WARNING", "Opening existing profiles file will override the existing profiles setup "
                                              "and previous configuration will be lost.\n"
                                              "Are you sure you want to continue?",
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (answer != QMessageBox::Yes) return;

    if (!QFile::copy(szFileName, m_qszProfilesPath))
    {
        qDebug() << "[MainWindow::onActionFileOpen] - Failed to copy file from '" << szFileName << "' to '" << m_qszProfilesPath << "'";
        return;
    }
}

void MainWindow::onActionFileSave()
{
}

void MainWindow::onActionFileSaveAs()
{
    QString szFileDstPath = QFileDialog::getSaveFileName(this, tr("Save As File"), "", tr("Profiles File (*.json)"));
    if (szFileDstPath.isEmpty()) return;

    if (!QFile::copy(m_qszProfilesPath, szFileDstPath))
    {
        qDebug() << "[MainWindow::onActionFileSaveAs] - Failed to copy file from '" << m_qszProfilesPath << "' to '" << szFileDstPath << "'";
        return;
    }
}

void MainWindow::onActionFileRestartService()
{
    QtServiceController service("QuickCut Service");
    service.stop();
    activateHook();
}

void MainWindow::onActionFileExit()
{
    close();
}

void MainWindow::onActionViewToolBar()
{
    const bool bChecked = ui->actionViewToolBar->isChecked();
    ui->toolBar->setVisible(bChecked);
    m_Preferences.m_bViewToolBar = bChecked;
    savePreferences();
}

void MainWindow::onActionViewStatusBar()
{
    const bool bChecked = ui->actionViewStatusBar->isChecked();
    ui->statusBar->setVisible(bChecked);
    m_Preferences.m_bViewStatusBar = bChecked;
    savePreferences();
}

void MainWindow::onActionViewRefresh()
{
    Hook::sendReloadSignal();
    initProfiles();
}

void MainWindow::onActionHelpAbout()
{
    m_pAboutWindow = new AboutWindow(this);
    m_pAboutWindow->exec();
}

void MainWindow::onActionHelpExamples()
{
    m_pExamplesWindow = new ExamplesWindow(this);
    m_pExamplesWindow->exec();
}

void MainWindow::onActionHelpCheckUpdates()
{
    m_pCheckUpdatesWindow = new CheckUpdatesWindow(this);
    m_pCheckUpdatesWindow->exec();
}

void MainWindow::onActionLoadTheme(eThemeType eType, QAction * pAction/*= nullptr*/)
{
    onActionLoadTheme(Preferences::getThemeFilePath(eType), pAction);
    m_Preferences.setThemeType(eType);
    savePreferences();
}
void MainWindow::onActionLoadTheme(const QString & szQssPath, QAction * pAction/*= nullptr*/)
{
    for (auto && qAction : m_qThemeActions)
        qAction->setChecked(false);

    if (pAction)
        pAction->setChecked(true);

    QFile file(szQssPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qApp->setStyleSheet("");
        return;
    }

    QTextStream ts(&file);
    qApp->setStyleSheet(ts.readAll());
}

void MainWindow::onLoadCustomStylesheet()
{
    QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Stylesheet File"), "", tr("Stylesheet File (*.qss)"));
    if (szFileName.isEmpty()) return;

    onActionLoadTheme(szFileName);
}
