
#include "pch.h"
#include "MainWindow.h"
#include "Types.h"
#include "Utils/Utility.h"

#include <QThread>
#include <QTimer>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <QtService/QtService>

#include "ActionEditWindow.h"
#include "AboutWindow.h"
#include "CheckUpdatesWindow.h"
#include "ExamplesWindow.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass)
    , m_ActionEditWindow(nullptr)
    , m_AboutWindow(nullptr)
    , m_CheckUpdatesWindow(nullptr)
    , m_ExamplesWindow(nullptr)
    , m_Preference("Config/preference.json")
{
    ui->setupUi(this);

    connectSlots();

    initPreference();
    initProfiles();
    activateHook();

    statusBar()->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_Profiles);
    m_Profiles.clear();
}

void MainWindow::connectSlots()
{
    // File Menu
    connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::onActionFileOpen);
    connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::onActionFileSave);
    connect(ui->actionFileSaveAs, &QAction::triggered, this, &MainWindow::onActionFileSaveAs);
    connect(ui->actionFileRestartService, &QAction::triggered, this,
            &MainWindow::onActionFileRestartService);
    connect(ui->actionFileExit, &QAction::triggered, this, &MainWindow::onActionFileExit);

    // View Menu
    connect(ui->actionViewToolBar, &QAction::triggered, this,
            &MainWindow::onActionViewToolBar);
    connect(ui->actionViewStatusBar, &QAction::triggered, this,
            &MainWindow::onActionViewStatusBar);
    connect(ui->actionViewRefresh, &QAction::triggered, this,
            &MainWindow::onActionViewRefresh);

    // Preference Menu
    connect(ui->actionThemeDefault, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeDefault, ui->actionThemeDefault); });
    connect(ui->actionThemeDark, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeDark, ui->actionThemeDark); });
    connect(ui->actionThemeBreezeDark, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeBreezeDark, ui->actionThemeBreezeDark); });
    connect(ui->actionThemeBreezeLight, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeBreezeLight, ui->actionThemeBreezeLight); });
    connect(ui->actionThemeConsoleDark, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeConsoleDark, ui->actionThemeConsoleDark); });
    connect(ui->actionThemeUbuntu, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeUbuntu, ui->actionThemeUbuntu); });
    connect(ui->actionThemeDarkOrange, &QAction::triggered, this,
            [this] { onActionLoadTheme(ThemeDarkOrange, ui->actionThemeDarkOrange); });
    m_ThemeActions.insert(ThemeDefault, ui->actionThemeDefault);
    m_ThemeActions.insert(ThemeDark, ui->actionThemeDark);
    m_ThemeActions.insert(ThemeBreezeDark, ui->actionThemeBreezeDark);
    m_ThemeActions.insert(ThemeBreezeLight, ui->actionThemeBreezeLight);
    m_ThemeActions.insert(ThemeConsoleDark, ui->actionThemeConsoleDark);
    m_ThemeActions.insert(ThemeUbuntu, ui->actionThemeUbuntu);
    m_ThemeActions.insert(ThemeDarkOrange, ui->actionThemeDarkOrange);

    connect(ui->actionThemeLoadQss, &QAction::triggered, this,
            &MainWindow::onLoadCustomStylesheet);

    // Help Menu
    connect(ui->actionHelpAbout, &QAction::triggered, this, &MainWindow::onActionHelpAbout);
    connect(ui->actionHelpExamples, &QAction::triggered, this,
            &MainWindow::onActionHelpExamples);
    connect(ui->actionHelpCheckUpdates, &QAction::triggered, this,
            &MainWindow::onActionHelpCheckUpdates);

    // Controls
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this,
            &MainWindow::onActionSelChange);
    connect(ui->lbxActions, &QListWidget::doubleClicked, this,
            &MainWindow::onActionDoubleClicked);
    connect(ui->cbxProfile, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onProfileSelChange);

    connect(ui->btnSetActive, &QPushButton::clicked, this, &MainWindow::onBtnSetActiveProfile);
    connect(ui->btnCreateProfile, &QPushButton::clicked, this,
            &MainWindow::onBtnCreateProfile);
    connect(ui->btnDeleteProfile, &QPushButton::clicked, this,
            &MainWindow::onBtnDeleteProfile);

    connect(ui->btnActionCreate, &QPushButton::clicked, this, &MainWindow::onBtnActionCreate);
    connect(ui->btnActionDelete, &QPushButton::clicked, this, &MainWindow::onBtnActionDelete);
    connect(ui->btnActionDuplicate, &QPushButton::clicked, this,
            &MainWindow::onBtnActionDuplicate);

    connect(ui->btnActionMoveDown, &QPushButton::clicked, this,
            &MainWindow::onBtnActionMoveDown);
    connect(ui->btnActionMoveUp, &QPushButton::clicked, this, &MainWindow::onBtnActionMoveUp);
}

void MainWindow::activateHook()
{
    QProcess::execute("QuickCutService -i", QStringList()); // TODO: Configure from installer.
    QtServiceController service("QuickCut Service");
    if (!service.isRunning())
    {
        service.start();
        ui->statusBar->showMessage("Activated QuickCut Service.");
    }

#if defined(Q_OS_WIN)
    // TODO: Configure from installer.
    // Don't start the service right when win boots.
    QProcess::execute("sc config \"QuickCut Service\" start=delayed-auto", QStringList());
    QProcess::execute("sc failure \"QuickCut Service\" actions=restart/60000/restart/60000/"
                      "/60000 reset=86400",
                      QStringList());

    const QString qcc = QCoreApplication::applicationDirPath() + "/QuickCutConsole.exe";

    // Approach 1:
    // QProcess::execute("schtasks /Create /TN \"QuickCut\" /SC ONLOGON /TR \"" + qcc + "\"");

    // Approach 2:
    // QSettings
    // settings(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)",
    // QSettings::NativeFormat); const QString value(settings.value("QuickCutConsole",
    // QString()).toString()); if (value.isEmpty() || value != qcc)
    //    settings.setValue("QuickCutConsole", qcc);

    // Approach 3: <- Simple and works.
    QDir dir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
    dir.cdUp();
    QFile::link(qcc, dir.path() +
                         "/Microsoft/Windows/Start Menu/Programs/Startup/QuickCutConsole.lnk");

    // Windows service starts the console hook as the default user using the winsta0\\default
    // which sometimes fails(QuickCutServiceWindows::RunProcessAsUserW), so if the user starts
    // QuickCut GUI, we use the chance to start QuickCutConsole as the current user just in
    // case.
    QTimer::singleShot(5000, [] {
        QThread * t = QThread::create([] {
            QProcess::execute("taskkill /im QuickCutConsole.exe /f", QStringList());
            QThread::msleep(500);
            QProcess::execute("QuickCutConsole", QStringList());
        });
        t->start();
    });
#endif
}

void MainWindow::showEvent(QShowEvent * event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::initPreference()
{
    QFileInfo fi(QString::fromStdString(m_Preference.getConfigFilePath()));

    bool createDefault = !fi.exists();
    if (createDefault)
    {
        if (!fi.dir().exists()) fi.dir().mkpath(".");

        m_Preference.save();
    }
    else
    {
        m_Preference.load();
    }

    QAction * action = nullptr;
    switch (m_Preference.get().getCurrentTheme())
    {
        case ThemeDefault:
            action = ui->actionThemeDefault;
            break;
        case ThemeDark:
            action = ui->actionThemeDark;
            break;
        case ThemeBreezeDark:
            action = ui->actionThemeBreezeDark;
            break;
        case ThemeBreezeLight:
            action = ui->actionThemeBreezeLight;
            break;
        case ThemeConsoleDark:
            action = ui->actionThemeConsoleDark;
            break;
        case ThemeUbuntu:
            action = ui->actionThemeUbuntu;
            break;
        case ThemeDarkOrange:
            action = ui->actionThemeDarkOrange;
            break;
        default:
            break;
    }

    onActionLoadTheme(m_Preference.get().getCurrentThemeResourcePath(), action);

    ui->actionViewToolBar->setChecked(m_Preference.get().isToolBarVisible());
    ui->actionViewStatusBar->setChecked(m_Preference.get().isStatusBarVisible());
    ui->toolBar->setVisible(m_Preference.get().isToolBarVisible());
    ui->statusBar->setVisible(m_Preference.get().isStatusBarVisible());
}

void MainWindow::initProfiles()
{
    ui->cbxProfile->clear();
    QFileInfo fi("Config/profiles.json");
    if (!fi.exists())
    {
        if (!fi.dir().exists()) fi.dir().mkdir(".");

        m_ProfilesPath = fi.filePath();
        onBtnCreateProfile();
        // First created profile, need to set as default.
        Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
        m_ActiveProfile   = QString::fromStdString(profile->getId());
        saveProfiles();
        ui->btnSetActive->setEnabled(false);
    }
    else
    {
        m_ProfilesPath = fi.filePath();

        std::string activeProfile;
        loadProfiles(m_ProfilesPath, &activeProfile);
        m_ActiveProfile = QString::fromStdString(activeProfile);

        for (auto && profile : m_Profiles)
        {
            QString profileName = QString::fromStdString(profile->getName());
            ui->cbxProfile->addItem(profileName);

            if (activeProfile != profile->getId()) continue; // Not active profile

            ui->lbxActions->clear();
            for (auto && action : profile->getActionManager())
                ui->lbxActions->addItem(QString::fromStdString(action->getName()));

            ui->cbxProfile->setCurrentText(profileName);
        }

        ui->lbxActions->setCurrentRow(0);
    }
}

#pragma region-- - Parser-- -
bool MainWindow::loadProfiles(const std::string & profilesPath, std::string * activeProfileOut)
{
    if (profilesPath.empty()) return false;

    JSON rootJson;
    bpt::read_json(profilesPath, rootJson);
    *activeProfileOut = rootJson.get<std::string>("activeProfile", "");
    int profileCount  = rootJson.get<int>("profileCount", 0);
    qDeleteAll(m_Profiles);
    m_Profiles.clear();
    m_Profiles.reserve(profileCount);

    JSON profilesJson = rootJson.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        std::string profileId    = profileJson.second.get<std::string>("id", "");
        std::string profileName  = profileJson.second.get<std::string>("name", "");
        std::string lastModified = profileJson.second.get<std::string>("lastModified", "");
        int         actionsCount = profileJson.second.get<int>("actionsCount", 0);

        Profile * profile = new Profile(profileId, profileName, lastModified);
        profile->getActionManager().setCapacity(actionsCount);

        JSON actionsJson = profileJson.second.get_child("actions");
        for (auto && actionJson : actionsJson)
        {
            std::string actionId     = actionJson.second.get<std::string>("id", "");
            std::string actionName   = actionJson.second.get<std::string>("actionName", "");
            std::string actionType   = actionJson.second.get<std::string>("type", "");
            std::string srcKey       = actionJson.second.get<std::string>("srcKey", "");
            std::string dstKey       = actionJson.second.get<std::string>("dstKey", "");
            std::string appPath      = actionJson.second.get<std::string>("appPath", "");
            std::string appArgs      = actionJson.second.get<std::string>("appArgs", "");
            std::string createdDate  = actionJson.second.get<std::string>("createdDate", "");
            std::string lastModified = actionJson.second.get<std::string>("lastModified", "");

            profile->getActionManager().add(new Action(actionId, actionName, lastModified,
                                                       Action::getType(actionType), srcKey,
                                                       dstKey, appPath, appArgs, createdDate));
        }

        m_Profiles.push_back(profile);
    }

    return true;
}

bool MainWindow::loadProfiles(const QString & profilesPath, QString * activeProfileOut)
{
    std::string activeProfile;
    if (!loadProfiles(profilesPath.toStdString(), &activeProfile)) return false;

    *activeProfileOut = QString::fromStdString(activeProfile);

    return true;
}

bool MainWindow::loadProfiles(const QString & profilesPath, std::string * activeProfileOut)
{
    return loadProfiles(profilesPath.toStdString(), activeProfileOut);
}

bool MainWindow::reloadProfiles()
{
    if (!loadProfiles(m_ProfilesPath, &m_ActiveProfile)) return false;

    onProfileSelChange(ui->cbxProfile->currentIndex());

    return true;
}

bool MainWindow::saveProfiles(const std::string & profilesPath,
                              const std::string & activeProfileId)
{
    if (profilesPath.empty() || activeProfileId.empty()) return false;

    JSON rootJson;
    rootJson.put("activeProfile", activeProfileId);
    rootJson.put("profileCount", m_Profiles.size());

    JSON profilesJson;
    for (auto && profile : m_Profiles)
    {
        JSON profileJson;
        profileJson.put("id", profile->getId());
        profileJson.put("name", profile->getName());
        profileJson.put("lastModified", profile->getLastModified());
        profileJson.put("actionsCount", profile->getActionManager().count());

        JSON actionsJson;
        for (auto && action : profile->getActionManager())
        {
            JSON actionJson;
            actionJson.put("id", action->getId());
            actionJson.put("actionName", action->getName());
            actionJson.put("type", Action::getType(action->getType()));
            actionJson.put("srcKey", action->getSrcKey());
            actionJson.put("dstKey", action->getDstKey());
            actionJson.put("appPath", action->getAppPath());
            actionJson.put("appArgs", action->getAppArgs());
            actionJson.put("createdDate", action->getCreatedDate());

            actionsJson.push_back(std::make_pair("", actionJson));
        }

        profileJson.push_back(std::make_pair("actions", actionsJson));
        profilesJson.push_back(std::make_pair("", profileJson));
    }
    rootJson.add_child("profiles", profilesJson);

    bpt::write_jsonEx(profilesPath, rootJson);
#if defined(Q_OS_WIN)
    Hook::sendReloadSignal();
#endif

    return true;
}

bool MainWindow::saveProfiles(const QString & profilesPath, const QString & activeProfileId)
{
    return saveProfiles(profilesPath.toStdString(), activeProfileId.toStdString());
}

bool MainWindow::saveProfiles()
{
    return saveProfiles(m_ProfilesPath, m_ActiveProfile);
}

// end  --- Parser ---
#pragma endregion

bool MainWindow::isActiveProfile(Profile * profile)
{
    return profile->getId() == m_ActiveProfile.toStdString();
}

void MainWindow::onProfileSelChange(int index)
{
    if (index < 0 || index > m_Profiles.size())
    {
        ui->lbxActions->clear();
        ui->btnSetActive->setEnabled(false);
        ui->btnDeleteProfile->setEnabled(false);
        ui->btnActionCreate->setEnabled(false);
        return;
    }

    Profile * profile = m_Profiles[index];
    ui->btnSetActive->setEnabled(!isActiveProfile(profile));
    ui->btnDeleteProfile->setEnabled(true);
    ui->btnActionCreate->setEnabled(true);

    disconnect(ui->lbxActions, &QListWidget::currentRowChanged, this,
               &MainWindow::onActionSelChange);
    ui->lbxActions->clear();
    for (auto && action : profile->getActionManager())
        ui->lbxActions->addItem(QString::fromStdString(action->getName()));
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this,
            &MainWindow::onActionSelChange);

    if (!profile->getActionManager().count())
        onActionSelChange(-1);
    else
        ui->lbxActions->setCurrentRow(0);
}

void MainWindow::onBtnSetActiveProfile()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    JSON      rootJson;
    bpt::read_json(m_ProfilesPath.toStdString(), rootJson);
    rootJson.put("activeProfile", profile->getId());
    bpt::write_jsonEx(m_ProfilesPath.toStdString(), rootJson);
    reloadProfiles();
    ui->btnSetActive->setEnabled(false);
}

void MainWindow::onBtnDeleteProfile()
{
    const int currIndex = ui->cbxProfile->currentIndex();
    Profile * profile   = m_Profiles[currIndex];
    ui->cbxProfile->removeItem(currIndex);
    if (isActiveProfile(profile) && m_Profiles.count() > 0)
    {
        m_ActiveProfile = QString::fromStdString(m_Profiles.first()->getId());
        delete profile;
        ui->cbxProfile->setCurrentIndex(0);
    }

    m_Profiles.removeAt(currIndex);

    saveProfiles();
    reloadProfiles();
}

void MainWindow::onBtnCreateProfile()
{
    bool    ok;
    QString profileName =
        QInputDialog::getText(this, tr("Create Profile"), tr("Profile Name:"),
                              QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (!ok && profileName.isEmpty()) return;

    const int index = ui->cbxProfile->count() > 0 ? ui->cbxProfile->currentIndex() + 1 : 0;

    Profile * profile = new Profile();
    profile->setName(profileName.toStdString());
    m_Profiles.push_back(profile);
    ui->cbxProfile->addItem(profileName);
    ui->cbxProfile->setCurrentIndex(index);
    saveProfiles();
}

void MainWindow::onActionSelChange(int index)
{
    const bool enabled = index != -1;
    ui->btnActionDelete->setEnabled(enabled);
    ui->btnActionDuplicate->setEnabled(enabled);
}

void MainWindow::onActionDoubleClicked(const QModelIndex & index)
{
    Profile * profile  = m_Profiles[ui->cbxProfile->currentIndex()];
    Action *  action   = profile->getActionManager().getByIndex(index.row());
    m_ActionEditWindow = new ActionEditWindow(this, action);
    connect(m_ActionEditWindow, &ActionEditWindow::onSaved, this, &MainWindow::onActionSaved);
    m_ActionEditWindow->exec();
}

void MainWindow::onBtnActionCreate()
{
    m_ActionEditWindow = new ActionEditWindow(this);
    connect(m_ActionEditWindow, &ActionEditWindow::onCreated, this,
            &MainWindow::onActionCreated);
    m_ActionEditWindow->exec();
}

void MainWindow::onBtnActionDelete()
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().remove(currIndex);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex - 1);
    if ((currIndex - 1) < 0) ui->btnActionCreate->setFocus();
}

void MainWindow::onBtnActionDuplicate()
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    Action *  action    = profile->getActionManager().getByIndex(currIndex);

    Action * newAction =
        new Action(action->getName(), action->getType(), action->getSrcKey(),
                   action->getDstKey(), action->getAppPath(), action->getAppArgs());

    profile->getActionManager().insert(currIndex, newAction);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex + 1);
}

void MainWindow::onBtnActionMoveDown()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().moveDown(ui->lbxActions->currentRow());

    listItemSwap(ui->lbxActions, false);
    saveProfiles();
}

void MainWindow::onBtnActionMoveUp()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().moveUp(ui->lbxActions->currentRow());

    listItemSwap(ui->lbxActions, true);
    saveProfiles();
}

void MainWindow::listItemSwap(QListWidget * list, const bool moveUp)
{
    const int currIndex = list->currentRow();
    if (currIndex == -1) return;

    if (moveUp)
    {
        if (currIndex > 0)
        {
            QListWidgetItem * pItem = list->takeItem(currIndex);
            list->insertItem(currIndex - 1, pItem);
            list->setCurrentRow(currIndex - 1);
        }
    }
    else
    {
        if (currIndex < list->count() - 1)
        {
            QListWidgetItem * pItem = list->takeItem(currIndex);
            list->insertItem(currIndex + 1, pItem);
            list->setCurrentRow(currIndex + 1);
        }
    }
}

void MainWindow::onActionSaved()
{
    const int currIndex = ui->lbxActions->currentRow();
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex);
}

void MainWindow::onActionCreated(Action * action)
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().add(action);
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex == -1 ? 0 : currIndex + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::onActionFileOpen()
{
    QString desktopDir =
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Profiles File"), desktopDir,
                                                    tr("Profiles File (*.json)"));
    if (fileName.isEmpty()) return;

    auto answer = QMessageBox::warning(
        this, "WARNING",
        "Opening existing profiles file will override the existing profiles setup "
        "and previous configuration will be lost.\n"
        "Are you sure you want to continue?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (answer != QMessageBox::Yes) return;

    if (!QFile::copy(fileName, m_ProfilesPath))
    {
        qDebug() << "[MainWindow::onActionFileOpen] - Failed to copy file from '" << fileName
                 << "' to '" << m_ProfilesPath << "'";
        return;
    }
}

void MainWindow::onActionFileSave() {}

void MainWindow::onActionFileSaveAs()
{
    QString fileDstPath = QFileDialog::getSaveFileName(this, tr("Save As File"), "",
                                                       tr("Profiles File (*.json)"));
    if (fileDstPath.isEmpty()) return;

    if (!QFile::copy(m_ProfilesPath, fileDstPath))
    {
        qDebug() << "[MainWindow::onActionFileSaveAs] - Failed to copy file from '"
                 << m_ProfilesPath << "' to '" << fileDstPath << "'";
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
    const bool checked = ui->actionViewToolBar->isChecked();
    ui->toolBar->setVisible(checked);
    m_Preference.get().setStatusBarVisible(checked);
    m_Preference.save();
}

void MainWindow::onActionViewStatusBar()
{
    const bool checked = ui->actionViewStatusBar->isChecked();
    ui->statusBar->setVisible(checked);
    m_Preference.get().setStatusBarVisible(checked);
    m_Preference.save();
}

void MainWindow::onActionViewRefresh()
{
#if defined(Q_OS_WIN)
    Hook::sendReloadSignal();
#endif
    initProfiles();
}

void MainWindow::onActionHelpAbout()
{
    m_AboutWindow = new AboutWindow(this);
    m_AboutWindow->exec();
}

void MainWindow::onActionHelpExamples()
{
    m_ExamplesWindow = new ExamplesWindow(this);
    m_ExamplesWindow->exec();
}

void MainWindow::onActionHelpCheckUpdates()
{
    m_CheckUpdatesWindow = new CheckUpdatesWindow(this);
    m_CheckUpdatesWindow->exec();
}

void MainWindow::onActionLoadTheme(ThemeType eType, QAction * action /*= nullptr*/)
{
    onActionLoadTheme(Preference::getThemeResourcePath(eType), action);
    m_Preference.get().setCurrentTheme(eType);
    m_Preference.save();
}
void MainWindow::onActionLoadTheme(const QString & szQssPath, QAction * action /*= nullptr*/)
{
    for (auto && qAction : m_ThemeActions) qAction->setChecked(false);

    if (action) action->setChecked(true);

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Stylesheet File"), "",
                                                    tr("Stylesheet File (*.qss)"));
    if (fileName.isEmpty()) return;

    onActionLoadTheme(fileName);
}
