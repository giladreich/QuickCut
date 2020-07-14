
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsole.h"

#include "QuickCutShared/Managers/ProfileManager.h"

#include <QClipboard>
#include <QImage>

QuickCutConsole * QuickCutConsole::s_Instance = nullptr;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QGuiApplication(argc, argv)
    , m_Profile(nullptr)
    , m_LocalSocket(new QLocalServer(this))
#if defined(Q_OS_WINDOWS)
    , m_Hook(new KeyboardHookWindows(true, false, this))
#else
    , m_Hook(new KeyboardHookUnix(true, false, this))
#endif
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    s_Instance = this;

    // Intercept changes coming from GUI locally
    if (!m_LocalSocket->listen(QUICKCUT_IPC))
    {
        qDebug() << "[QuickCutConsole::ctor]: Failed to start IPC local server -> "
                 << m_LocalSocket->errorString();
    }

    connect(m_LocalSocket, &QLocalServer::newConnection, this, &QuickCutConsole::loadProfiles);
    connect(m_Hook, &KeyboardHook::keysDown, this, &QuickCutConsole::onKeysDown);
    connect(m_Hook, &KeyboardHook::keyUp, this, &QuickCutConsole::onKeyUp);

#if defined(Q_OS_MACOS)
    // TODO(Gilad): Add CMD + V keys pattern.
#elif defined(Q_OS_WIN)
    m_PasteKeys.push_back(KeyData("Ctrl", VK_LCONTROL));
    m_PasteKeys.push_back(KeyData("V", 0x56));
#elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
    // TODO(Gilad): Add CTRL + V keys pattern.
#endif
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

void QuickCutConsole::onKeysDown(const KeyboardKeys & keys, bool * outSwallowKey)
{
    if (!m_Profile || m_Profile->getActionManager().empty() || keys.isEmpty()) return;

    QString downKeyCodes = Action::getKeysCode(keys);
    for (auto && action : m_Profile->getActionManager())
    {
        if (!action->isEnabled()) continue;

        QString srcKeyCodes = action->getSrcKeysCode();
        if (downKeyCodes == srcKeyCodes)
        {
            Action::ActionType actionType = action->getType();
            if (actionType == Action::ActionKeyMap)
            {
                qDebug() << "Mapped keys -> " << qPrintable(downKeyCodes) << " ~ "
                         << qPrintable(action->getDstKeysCode());

                *outSwallowKey = true;
                // Send KeyUp to the current pressed keys before manipulating the fake keys.
                sendInput(action->getSrcKeys(), KeyboardHook::KeyUp);
                if (action->getDstKeys().count() > 1)
                    sendInput(action->getDstKeys(), KeyboardHook::KeyPress);
                else
                    sendInput(action->getDstKeys(), KeyboardHook::KeyDown);
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
            else if (actionType == Action::ActionAutoText)
            {
                qDebug() << "Pasting auto text action -> " << action->getAutoText();

                *outSwallowKey = true;
                sendInput(action->getSrcKeys(), KeyboardHook::KeyUp);

                auto cp       = QGuiApplication::clipboard();
                auto cpBackup = new QMimeData();
                if (cp->mimeData())
                {
                    auto mime = cp->mimeData();
                    for (auto && format : mime->formats())
                    {
                        if (format == "application/x-qt-image")
                            cpBackup->setImageData(cp->image());
                        else
                            cpBackup->setData(format, mime->data(format));
                    }
                }

                cp->setText(action->getAutoText());
                sendInput(m_PasteKeys, KeyboardHook::KeyPress);
                QTimer::singleShot(50, [=] { cp->setMimeData(cpBackup); });
            }
        }
    }
}

void QuickCutConsole::onKeyUp(const KeyData & key, bool * outSwallowKey)
{
    if (!m_Profile || m_Profile->getActionManager().empty()) return;

    QString upKeyCode = QString::number(key.getKeyCode(), 16).toUpper();
    for (auto && action : m_Profile->getActionManager())
    {
        // For multiple key mapping, we send a KeyPress event, which end-up sending
        // KeyDown+KeyUp, but for a singular key mapping, we send a KeyDown event onKeysDown,
        // in order to fully simulate the original keyboard behavior. Therefore, we only need
        // to send a KeyUp event when dstKeysCount is 1.
        if (action->getDstKeys().count() != 1 || !action->isEnabled()) continue;

        QString srcKeyCodes = action->getSrcKeysCode();
        if (upKeyCode == srcKeyCodes)
        {
            if (action->getType() == Action::ActionKeyMap)
            {
                qDebug() << "Mapped KeyUp keys -> " << qPrintable(upKeyCode) << " ~ "
                         << qPrintable(action->getDstKeysCode());

                *outSwallowKey = true;
                sendInput(action->getDstKeys(), KeyboardHook::KeyUp);
            }
        }
    }
}