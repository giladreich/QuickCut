
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutConsole.h"

#include "QuickCutShared/Managers/ProfileManager.h"

QuickCutConsole * QuickCutConsole::s_Instance    = nullptr;
QLocalServer *    QuickCutConsole::s_LocalSocket = nullptr;

Profile *      QuickCutConsole::s_Profile = nullptr;
ProfileManager QuickCutConsole::s_ProfileManager;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
{
    s_Instance = this;

    // Intercept changes coming from GUI locally
    s_LocalSocket = new QLocalServer(this);
    if (!s_LocalSocket->listen(QUICKCUT_IPC))
    { qDebug() << "Failed to start IPC local server -> " << s_LocalSocket->errorString(); }

    connect(s_LocalSocket, &QLocalServer::newConnection, this, &QuickCutConsole::loadProfiles);
}

QuickCutConsole::~QuickCutConsole() = default;

bool QuickCutConsole::start()
{
    // The QuickCut GUI every changes are made to our profiles, we'll restart the service to
    // reload profiles.
    if (!loadProfiles()) return false;

    return true;
}

bool QuickCutConsole::stop()
{
    return true;
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

    s_Profile       = s_ProfileManager.getActiveProfile();
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

void QuickCutConsole::log(const QString & filePath, const QString & text)
{
    QFile file(filePath);
    file.open(QFile::WriteOnly);
    QTextStream ts(&file);
    ts << text;
    file.flush();
    file.close();
}
