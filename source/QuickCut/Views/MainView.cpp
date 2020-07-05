
#include "QuickCutShared/QuickCutPCH.h"
#include "MainView.h"

#include "ActionView.h"
#include "AboutView.h"
#include "UpdatesView.h"
#include "ExamplesView.h"

#include "QuickCutShared/QtService/QtService.h"

#include <QThread>
#include <QTimer>
#include <QInputDialog>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

MainView::MainView(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainView)
    , m_ActionView(nullptr)
    , m_AboutView(nullptr)
    , m_UpdatesView(nullptr)
    , m_ExamplesView(nullptr)
    , m_Profiles("Config/profiles.json")
    , m_Preference("Config/preference.json")
    , m_LocalSocket(new QLocalSocket(this))
    , m_SocketBlockSize(0)
{
    ui->setupUi(this);

    m_SocketStreamIn.setDevice(m_LocalSocket);
    m_SocketStreamIn.setVersion(QDataStream::Qt_5_15);

    initThemes();
    initPreference();
    initProfiles();
    activateHook();

    connectSlots();
    statusBar()->showMessage("Ready.");
}

MainView::~MainView()
{
    m_Profiles.clear();
}

void MainView::connectSlots()
{
    connect(m_LocalSocket, &QLocalSocket::readyRead, this,
            &MainView::onReloadProfilesResponse);

    // File Menu
    connect(ui->actionFileOpen, &QAction::triggered, this, &MainView::onActionFileOpen);
    connect(ui->actionFileSave, &QAction::triggered, this, &MainView::onActionFileSave);
    connect(ui->actionFileSaveAs, &QAction::triggered, this, &MainView::onActionFileSaveAs);
    connect(ui->actionFileRestartService, &QAction::triggered, this,
            &MainView::onActionFileRestartService);
    connect(ui->actionFileExit, &QAction::triggered, this, &MainView::onActionFileExit);

    // View Menu
    connect(ui->actionViewToolBar, &QAction::triggered, this, &MainView::onActionViewToolBar);
    connect(ui->actionViewStatusBar, &QAction::triggered, this,
            &MainView::onActionViewStatusBar);
    connect(ui->actionViewRefresh, &QAction::triggered, this, &MainView::onActionViewRefresh);

    // Preference Menu
    connect(ui->actionThemeDefault, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeDefault); });
    connect(ui->actionThemeDark, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeDark); });
    connect(ui->actionThemeBreezeDark, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeBreezeDark); });
    connect(ui->actionThemeBreezeLight, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeBreezeLight); });
    connect(ui->actionThemeConsoleDark, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeConsoleDark); });
    connect(ui->actionThemeUbuntu, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeUbuntu); });
    connect(ui->actionThemeDarkOrange, &QAction::triggered, this,
            [&] { onActionLoadTheme(QCTheme::ThemeDarkOrange); });
    connect(ui->actionThemeLoadQss, &QAction::triggered, this,
            &MainView::onLoadCustomStylesheet);

    // Help Menu
    connect(ui->actionHelpAbout, &QAction::triggered, this, &MainView::onActionHelpAbout);
    connect(ui->actionHelpExamples, &QAction::triggered, this,
            &MainView::onActionHelpExamples);
    connect(ui->actionHelpCheckUpdates, &QAction::triggered, this,
            &MainView::onActionHelpCheckUpdates);

    // Controls
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this,
            &MainView::onActionSelChange);
    connect(ui->lbxActions, &QListWidget::doubleClicked, this,
            &MainView::onActionDoubleClicked);
    connect(ui->cbxProfile, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainView::onProfileSelChange);

    connect(ui->btnSetActive, &QPushButton::clicked, this, &MainView::onBtnSetActiveProfile);
    connect(ui->btnCreateProfile, &QPushButton::clicked, this, &MainView::onBtnCreateProfile);
    connect(ui->btnDeleteProfile, &QPushButton::clicked, this, &MainView::onBtnDeleteProfile);

    connect(ui->btnActionCreate, &QPushButton::clicked, this, &MainView::onBtnActionCreate);
    connect(ui->btnActionDelete, &QPushButton::clicked, this, &MainView::onBtnActionDelete);
    connect(ui->btnActionDuplicate, &QPushButton::clicked, this,
            &MainView::onBtnActionDuplicate);

    connect(ui->btnActionMoveDown, &QPushButton::clicked, this,
            &MainView::onBtnActionMoveDown);
    connect(ui->btnActionMoveUp, &QPushButton::clicked, this, &MainView::onBtnActionMoveUp);
}

// TODO(Gilad): Consider moving everything to QuickCutShared/Utils/HookHelper class.
void MainView::activateHook()
{
    QtServiceController service(QUICKCUTSERVICE_NAME);
    if (!service.isInstalled())
    {
        if (service.install(QUICKCUTSERVICE_BIN))
            qDebug() << "[MainView::activateHook]: Successfully installed the service.";
        else
            qDebug() << "[MainView::activateHook]: Failed to install the service.";
    }

    if (!service.isRunning())
    {
        if (service.start())
        {
            qDebug() << "[MainView::activateHook]: Successfully started the service.";
            statusBar()->showMessage("Successfully started the service.");
        }
        else
        {
            qDebug() << "[MainView::activateHook]: Failed to start the service.";
            statusBar()->showMessage("Failed to start the service.");
        }
    }

#if defined(Q_OS_WIN)
    // TODO: Configure from installer.
    // Don't start the service right when the system boots, so it gives the startup application
    // to start the hook before the service tries.
    QString command =
        QString(R"(sc config "%1" start=delayed-auto)").arg(QUICKCUTSERVICE_NAME);
    WinExec(qPrintable(command), SW_HIDE);
    command =
        QString(
            R"(sc failure "%1" actions=restart/60000ms/restart/60000ms/restart/60000ms// reset=86400)")
            .arg(QUICKCUTSERVICE_NAME);
    WinExec(qPrintable(command), SW_HIDE);

    QDir    qccDir(qApp->applicationDirPath());
    QString qcc = qccDir.filePath(QUICKCUTCONSOLE_BIN);

    // Approach 1:
    // command =
    //    QString(R"(schtasks /Create /TN "%1" /SC ONLOGON /TR "%2")").arg(QUICKCUT_NAME, qcc);
    // WinExec(qPrintable(command), SW_HIDE);

    // Approach 2:
    // QSettings
    // settings(R"(HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)",
    //                   QSettings::NativeFormat);
    // QString   value(settings.value(QUICKCUTCONSOLE_BIN, QString()).toString());
    // if (value.isEmpty() || value != qcc) settings.setValue(QUICKCUTCONSOLE_BIN, qcc);

    // Approach 3: <- Simple and works.
    QString appsPath = QStandardPaths::locate(QStandardPaths::ApplicationsLocation, "Startup",
                                              QStandardPaths::LocateDirectory);
    if (!appsPath.isEmpty())
    {
        QDir appsDir(appsPath);
        QFile::link(qcc, appsDir.filePath(QUICKCUTCONSOLE_NAME ".lnk"));
    }

    // Windows service starts the console hook as the default user using the winsta0\\default
    // which sometimes fails(QuickCutServiceWindows::RunProcessAsUserW), so if the user starts
    // QuickCut GUI, we use the chance to start QuickCutConsole as the current user.
    WinExec("taskkill /f /t /im " QUICKCUTCONSOLE_BIN, SW_HIDE);
    QTimer::singleShot(5000, [] {
        QThread * t = QThread::create([] {
            QThread::msleep(1000);
            WinExec(QUICKCUTCONSOLE_BIN, SW_HIDE);
        });
        t->start();
    });
#endif
}

void MainView::showEvent(QShowEvent * event)
{
    QMainWindow::showEvent(event);
}

void MainView::initThemes()
{
    m_ThemeActions.insert(QCTheme::ThemeDefault, ui->actionThemeDefault);
    m_ThemeActions.insert(QCTheme::ThemeDark, ui->actionThemeDark);
    m_ThemeActions.insert(QCTheme::ThemeBreezeDark, ui->actionThemeBreezeDark);
    m_ThemeActions.insert(QCTheme::ThemeBreezeLight, ui->actionThemeBreezeLight);
    m_ThemeActions.insert(QCTheme::ThemeConsoleDark, ui->actionThemeConsoleDark);
    m_ThemeActions.insert(QCTheme::ThemeUbuntu, ui->actionThemeUbuntu);
    m_ThemeActions.insert(QCTheme::ThemeDarkOrange, ui->actionThemeDarkOrange);
}

void MainView::initPreference()
{
    if (!m_Preference.load()) m_Preference.save();

    auto currTheme = m_Preference.get().theme().get();
    onActionLoadTheme(currTheme);

    ui->actionViewToolBar->setChecked(m_Preference.get().isToolBarVisible());
    ui->actionViewStatusBar->setChecked(m_Preference.get().isStatusBarVisible());
    ui->toolBar->setVisible(m_Preference.get().isToolBarVisible());
    ui->statusBar->setVisible(m_Preference.get().isStatusBarVisible());
}

void MainView::initProfiles()
{
    ui->lbxActions->clear();
    ui->cbxProfile->clear();

    if (!m_Profiles.load() || m_Profiles.empty())
    {
        Profile * profile = onBtnCreateProfile();
        if (profile)
            m_Profiles.setActiveProfile(profile);
        else
            onProfileSelChange(-1);

        onActionSelChange(-1);
        saveProfiles();
    }
    else
    {
        for (auto && profile : m_Profiles) ui->cbxProfile->addItem(profile->getName());

        Profile * profile = m_Profiles.getActiveProfile();
        if (profile)
        {
            ui->cbxProfile->setCurrentText(profile->getName());
            if (!profile->getActionManager().empty())
            {
                for (auto && action : profile->getActionManager())
                    ui->lbxActions->addItem(action->getName());
                onActionSelChange(0);
            }
            else
                onActionSelChange(-1);
        }
    }

    ui->btnSetActive->setEnabled(false);
}

bool MainView::reloadProfiles()
{
    if (!loadProfiles()) return false;

    onProfileSelChange(ui->cbxProfile->currentIndex());

    return true;
}

bool MainView::loadProfiles()
{
    return m_Profiles.load();
}

bool MainView::saveProfiles()
{
    bool result = m_Profiles.save();
    if (result) sendReloadProfiles();
    return result;
}

bool MainView::sendReloadProfiles()
{
    m_SocketBlockSize = 0;
    m_LocalSocket->abort();
    m_LocalSocket->connectToServer(QUICKCUT_IPC);
    return true;
}

void MainView::onReloadProfilesResponse()
{
    if (m_SocketBlockSize == 0)
    {
        if ((int)m_LocalSocket->bytesAvailable() < sizeof(int)) return;
        m_SocketStreamIn >> m_SocketBlockSize;
    }

    if (m_LocalSocket->bytesAvailable() < m_SocketBlockSize || m_SocketStreamIn.atEnd())
        return;

    QString responseMessage;
    m_SocketStreamIn >> responseMessage;
    qDebug() << "[MainView::onReloadProfilesResponse]: Response -> "
             << qPrintable(responseMessage);
}

void MainView::onProfileSelChange(int index)
{
    ui->lbxActions->clear();
    Profile * profile = m_Profiles[index];
    if (!profile)
    {
        ui->btnSetActive->setEnabled(false);
        ui->btnDeleteProfile->setEnabled(false);
        ui->btnActionCreate->setEnabled(false);
        ui->btnActionDelete->setEnabled(false);
        ui->btnActionDuplicate->setEnabled(false);
        return;
    }

    ui->btnSetActive->setEnabled(!profile->isActive());
    ui->btnDeleteProfile->setEnabled(true);
    ui->btnActionCreate->setEnabled(true);

    disconnect(ui->lbxActions, &QListWidget::currentRowChanged, this,
               &MainView::onActionSelChange);
    for (auto && action : profile->getActionManager())
        ui->lbxActions->addItem(action->getName());
    connect(ui->lbxActions, &QListWidget::currentRowChanged, this,
            &MainView::onActionSelChange);

    if (profile->getActionManager().empty())
        onActionSelChange(-1);
    else
        ui->lbxActions->setCurrentRow(0);
}

void MainView::onBtnSetActiveProfile()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    if (!profile) return;

    m_Profiles.setActiveProfile(profile);
    saveProfiles();
    reloadProfiles();
}

void MainView::onBtnDeleteProfile()
{
    const int currIndex = ui->cbxProfile->currentIndex();
    Profile * profile   = m_Profiles[currIndex];

    bool wasActiveProfile = profile->isActive();
    m_Profiles.remove(currIndex);
    ui->cbxProfile->removeItem(currIndex);
    if (wasActiveProfile && m_Profiles.count() > 0)
    {
        m_Profiles.setActiveProfile(0);
        ui->cbxProfile->setCurrentIndex(0);
    }
    saveProfiles();
    reloadProfiles();
}

Profile * MainView::onBtnCreateProfile()
{
    bool    ok;
    QString profileName = QInputDialog::getText(
        this, tr("Create Profile"), tr("Profile Name:"), QLineEdit::Normal,
        QDir::home().dirName(), &ok,
        Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    if (!ok && profileName.isEmpty()) return nullptr;

    Profile * profile = m_Profiles.getByName(profileName);
    if (profile)
    {
        QMessageBox::information(this, "Already Exists",
                                 "The given profile name already exists.",
                                 QMessageBox::StandardButton::Ok);
        return nullptr;
    }

    profile = new Profile();
    profile->setName(profileName);
    if (m_Profiles.empty()) profile->setActive(true);
    m_Profiles.add(profile);
    saveProfiles();

    ui->cbxProfile->addItem(profileName);
    ui->cbxProfile->setCurrentText(profileName);

    return profile;
}

void MainView::onActionSelChange(int index)
{
    bool enabled = index >= 0;
    if (!enabled) ui->btnActionCreate->setFocus();
    ui->btnActionDelete->setEnabled(enabled);
    ui->btnActionDuplicate->setEnabled(enabled);

    ui->btnActionMoveUp->setEnabled(enabled);
    ui->btnActionMoveDown->setEnabled(enabled);
}

void MainView::onActionDoubleClicked(const QModelIndex & index)
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    Action *  action  = profile->getActionManager()[index.row()];

    m_ActionView = new ActionView(this, action);
    connect(m_ActionView, &ActionView::onSaved, this, &MainView::onActionSaved);
    m_ActionView->exec();
}

void MainView::onBtnActionCreate()
{
    m_ActionView = new ActionView(this);
    connect(m_ActionView, &ActionView::onCreated, this, &MainView::onActionCreated);
    m_ActionView->exec();
}

void MainView::onBtnActionDelete()
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().remove(currIndex);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex - 1);
}

void MainView::onBtnActionDuplicate()
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    Action *  action    = profile->getActionManager().getByIndex(currIndex);

    Action * newAction =
        new Action(action->getName(), action->getType(), action->getSrcKeys(),
                   action->getDstKeys(), action->getTargetPath(), action->getAppArgs());

    profile->getActionManager().insert(currIndex, newAction);

    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex + 1);
}

void MainView::onBtnActionMoveDown()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().moveDown(ui->lbxActions->currentRow());

    listItemSwap(ui->lbxActions, false);
    saveProfiles();
}

void MainView::onBtnActionMoveUp()
{
    Profile * profile = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().moveUp(ui->lbxActions->currentRow());

    listItemSwap(ui->lbxActions, true);
    saveProfiles();
}

void MainView::listItemSwap(QListWidget * list, bool moveUp)
{
    const int currIndex = list->currentRow();
    if (currIndex == -1) return;

    if (moveUp)
    {
        if (currIndex > 0)
        {
            QListWidgetItem * item = list->takeItem(currIndex);
            list->insertItem(currIndex - 1, item);
            list->setCurrentRow(currIndex - 1);
        }
    }
    else
    {
        if (currIndex < list->count() - 1)
        {
            QListWidgetItem * item = list->takeItem(currIndex);
            list->insertItem(currIndex + 1, item);
            list->setCurrentRow(currIndex + 1);
        }
    }
}

void MainView::onActionSaved()
{
    const int currIndex = ui->lbxActions->currentRow();
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow(currIndex);
}

void MainView::onActionCreated(Action * action)
{
    const int currIndex = ui->lbxActions->currentRow();
    Profile * profile   = m_Profiles[ui->cbxProfile->currentIndex()];
    profile->getActionManager().add(action);
    saveProfiles();
    reloadProfiles();
    ui->lbxActions->setCurrentRow((currIndex >= 0) ? (currIndex + 1) : 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainView::onActionFileOpen()
{
    QString desktopDir =
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();
    QString srcFilePath = QFileDialog::getOpenFileName(
        this, tr("Open Profiles File"), desktopDir, tr("Profiles File (*.json)"));
    if (srcFilePath.isEmpty()) return;

    auto answer = QMessageBox::warning(
        this, "WARNING",
        "Opening existing profiles file will override the existing profiles\n"
        "Are you sure you want to continue?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (answer != QMessageBox::Yes) return;

    QString dstFilePath = m_Profiles.getConfigFilePath();
    if (!QFile::copy(srcFilePath, dstFilePath))
    {
        qDebug() << "[MainView::onActionFileOpen] - Failed to copy file from '" << srcFilePath
                 << "' to '" << dstFilePath << "'";
        return;
    }
}

void MainView::onActionFileSave()
{
    m_Profiles.save();
    m_Preference.save();
}

void MainView::onActionFileSaveAs()
{
    QString dstFilePath = QFileDialog::getSaveFileName(this, tr("Save As File"), "",
                                                       tr("Profiles File (*.json)"));
    if (dstFilePath.isEmpty()) return;

    QString srcFilePath = m_Profiles.getConfigFilePath();
    if (!QFile::copy(srcFilePath, dstFilePath))
    {
        qDebug() << "[MainView::onActionFileSaveAs] - Failed to copy file from '"
                 << srcFilePath << "' to '" << dstFilePath << "'";
        return;
    }
}

void MainView::onActionFileRestartService()
{
    QtServiceController service(QUICKCUTSERVICE_NAME);
    service.stop();
    QTimer::singleShot(1000, [&] { activateHook(); });
}

void MainView::onActionFileExit()
{
    close();
}

void MainView::onActionViewToolBar()
{
    bool visible = ui->actionViewToolBar->isChecked();
    ui->toolBar->setVisible(visible);
    m_Preference.get().setToolBarVisible(visible);
    m_Preference.save();
}

void MainView::onActionViewStatusBar()
{
    bool visible = ui->actionViewStatusBar->isChecked();
    ui->statusBar->setVisible(visible);
    m_Preference.get().setStatusBarVisible(visible);
    m_Preference.save();
}

void MainView::onActionViewRefresh()
{
    sendReloadProfiles();
    initProfiles();
}

void MainView::onActionHelpAbout()
{
    m_AboutView = new AboutView(this);
    m_AboutView->exec();
}

void MainView::onActionHelpExamples()
{
    m_ExamplesView = new ExamplesView(this);
    m_ExamplesView->exec();
}

void MainView::onActionHelpCheckUpdates()
{
    m_UpdatesView = new UpdatesView(this);
    m_UpdatesView->exec();
}

void MainView::onActionLoadTheme(QCTheme::ThemeType type)
{
    onActionLoadTheme(QCTheme::getResourcePath(type), m_ThemeActions[type]);
    m_Preference.get().getTheme().set(type);
    m_Preference.save();
}
void MainView::onActionLoadTheme(const QString & qssPath, QAction * action /*= nullptr*/)
{
    for (auto && qAction : m_ThemeActions) qAction->setChecked(false);

    if (action) action->setChecked(true);

    QFile file(qssPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qApp->setStyleSheet("");
        return;
    }

    QTextStream ts(&file);
    qApp->setStyleSheet(ts.readAll());
}

void MainView::onLoadCustomStylesheet()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Stylesheet File"), "",
                                                    tr("Stylesheet File (*.qss)"));
    if (filePath.isEmpty()) return;

    onActionLoadTheme(filePath);
}
