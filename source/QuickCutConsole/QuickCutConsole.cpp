
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsole.h"

#include "QuickCutShared/Managers/ProfileManager.h"

QuickCutConsole * QuickCutConsole::s_Instance = nullptr;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
    , m_Profile(nullptr)
    , m_LocalSocket(new QLocalServer(this))
#if defined(Q_OS_WINDOWS)
    , m_Hook(new KeyboardHookWindows(true, this))
#else
    , m_Hook(new KeyboardHookUnix(true, this))
#endif
{
    s_Instance = this;

    // Intercept changes coming from GUI locally
    if (!m_LocalSocket->listen(QUICKCUT_IPC))
    {
        qDebug() << "[QuickCutConsole::ctor]: Failed to start IPC local server -> "
                 << m_LocalSocket->errorString();
    }

    connect(m_LocalSocket, &QLocalServer::newConnection, this, &QuickCutConsole::loadProfiles);
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
    if (!m_ProfileManager.load())
    {
        QString message = QString("[QuickCutConsole::loadProfiles]: Failed to load %1 file.")
                              .arg(m_ProfileManager.getConfigFilePath());
        notifyStatusToClient(message);
        qDebug() << qPrintable(message);
        return false;
    }
    m_Profile = m_ProfileManager.getActiveProfile();

    QString message = "[QuickCutConsole::loadProfiles]: Successfully reloaded profiles.";
    notifyStatusToClient(message);
    qDebug() << qPrintable(message);
    return true;
}

bool QuickCutConsole::notifyStatusToClient(const QString & message)
{
    if (!m_LocalSocket->hasPendingConnections()) return false;

    QByteArray  block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << message.size();
    out << message;

    QLocalSocket * clientConnection = m_LocalSocket->nextPendingConnection();
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

void QuickCutConsole::onKeysPress(const KeyboardKeys & keys, bool * outSwallowKey)
{
    if (!m_Profile || m_Profile->getActionManager().empty() || keys.isEmpty()) return;

    QString pressedKeyCodes = Action::getKeysCode(keys);
    for (auto && action : m_Profile->getActionManager())
    {
        if (!action->isEnabled()) continue;

        QString srcKeyCodes = action->getSrcKeysCode();
        if (pressedKeyCodes == srcKeyCodes)
        {
            qDebug() << "Found match: " << pressedKeyCodes;
            Action::ActionType actionType = action->getType();
            if (actionType == Action::ActionKeyMap)
            {
                qDebug() << "Mapped keys -> " << qPrintable(pressedKeyCodes) << " ~ "
                         << qPrintable(action->getDstKeysCode());

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
                executeProcess(action->getTargetPath(), QString());
            }
        }
    }
}
