
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsole.h"

#include "QuickCutShared/Managers/ProfileManager.h"

QuickCutConsole * QuickCutConsole::s_Instance    = nullptr;
QLocalServer *    QuickCutConsole::s_LocalSocket = nullptr;

Profile *      QuickCutConsole::s_Profile = nullptr;
ProfileManager QuickCutConsole::s_ProfileManager;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
#if defined(Q_OS_WINDOWS)
    , m_Hook(new KeyboardHookWindows(true, this))
#else
    , m_Hook(new KeyboardHookUnix(true, this))
#endif
{
    s_Instance = this;

    // Intercept changes coming from GUI locally
    s_LocalSocket = new QLocalServer(this);
    if (!s_LocalSocket->listen(QUICKCUT_IPC))
    {
        qDebug() << "[QuickCutConsole::ctor]: Failed to start IPC local server -> "
                 << s_LocalSocket->errorString();
    }

    connect(s_LocalSocket, &QLocalServer::newConnection, this, &QuickCutConsole::loadProfiles);
    connect(m_Hook, &KeyboardHook::keysPressed, this, &QuickCutConsole::onKeysPress);
}

QuickCutConsole::~QuickCutConsole() = default;

bool QuickCutConsole::start()
{
    if (!loadProfiles()) return false;

    return m_Hook->activateHook();
}

bool QuickCutConsole::stop()
{
    return m_Hook->deactivateHook();
}

bool QuickCutConsole::loadProfiles()
{
    if (!s_ProfileManager.load())
    {
        QString message = QString("[QuickCutConsole::loadProfiles]: Failed to load %1 file.")
                              .arg(s_ProfileManager.getConfigFilePath());
        notifyStatusToClient(message);
        qDebug() << qPrintable(message);
        return false;
    }
    s_Profile = s_ProfileManager.getActiveProfile();

    QString message = "[QuickCutConsole::loadProfiles]: Successfully reloaded profiles.";
    notifyStatusToClient(message);
    qDebug() << qPrintable(message);
    return true;
}

bool QuickCutConsole::notifyStatusToClient(const QString & message)
{
    if (!s_LocalSocket->hasPendingConnections()) return false;

    QByteArray  block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << message.size();
    out << message;

    QLocalSocket * clientConnection = s_LocalSocket->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::disconnected, clientConnection,
            &QLocalSocket::deleteLater);

    clientConnection->write(block);
    bool succeed = clientConnection->flush();
    clientConnection->disconnectFromServer();
    if (!succeed)
        qDebug()
            << "[QuickCutConsole::notifyStatusToClient]: Failed to send message to client.";

    return succeed;
}

void QuickCutConsole::onKeysPress(const QStringList & keys, bool * outSwallowKey)
{
    if (s_Profile || s_Profile->getActionManager().empty() || keys.isEmpty()) return;

    ActionManager & actions         = s_Profile->getActionManager();
    QString         pressedKeyCodes = keys.join(QLatin1Char('+'));
    pressedKeyCodes                 = pressedKeyCodes.left(pressedKeyCodes.length() - 1);
    for (auto && action : actions)
    {
        QString srcKeyCodes = action->getSrcKey();
        if (pressedKeyCodes == srcKeyCodes)
        {
            qDebug() << "Found match: " << pressedKeyCodes;
            Action::ActionType actionType = action->getType();
            if (actionType == Action::ActionKeyMap)
            {
                qDebug() << "Mapped keys -> " << qPrintable(pressedKeyCodes) << " ~ "
                         << qPrintable(action->getDstKey());

                *outSwallowKey = true;
                // sendInput(action->getDstKey());
            }
            else if (actionType == Action::ActionAppLaunch)
            {
                qDebug() << "Launch process -> " << action->getTargetPath();
                executeProcess(action->getTargetPath(), action->getAppArgs());
            }
            else if (actionType == Action::ActionDirLaunch)
            {
                qDebug() << "Launch directory -> " << action->getTargetPath();
                executeProcess(action->getTargetPath(), "");
            }
        }
    }
}

void QuickCutConsole::executeProcess(const QString & process, const QString & arguments)
{
#if defined(Q_OS_WIN)
    QString command = QString(R"(cmd /c start "" "%1")").arg(process);
#elif defined(Q_OS_UNIX)
    QString command = QString(R"(sh -c '%1')").arg(process);
#endif

    QStringList argsTmp = arguments.trimmed().split(",");
    for (auto && arg : argsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        command += " " + argTrimmed;
    }
    qDebug() << "[QuickCutConsole::executeProcess] - Execute Command: "
             << "[" << qPrintable(command) << "]";

#if defined(Q_OS_WIN)
    WinExec(qPrintable(command), SW_HIDE);
#elif defined(Q_OS_UNIX)
    // Writing as script temporary to disk to avoid any white spaces issues
    // that QProcess doesn't handle very well...
    QString filePath = applicationDirPath() + "/tempCmd.sh";
    QFile   scriptFile(filePath);
    scriptFile.open(QIODevice::ReadWrite);
    QTextStream ts(&scriptFile);
    ts << command;
    scriptFile.close();
    QProcess::execute(filePath, QStringList());
    scriptFile.remove();
#endif
}
