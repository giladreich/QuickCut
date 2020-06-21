
#include "pch.h"
#include "QuickCutConsole.h"
#include "Managers/ProfileManager.h"

QuickCutConsole * QuickCutConsole::s_Instance = nullptr;

Profile *      QuickCutConsole::s_Profile = nullptr;
ProfileManager QuickCutConsole::s_ProfileManager;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
{
    if (!s_Instance) s_Instance = this;
}

QuickCutConsole::~QuickCutConsole()
{
    s_Profile = nullptr;
    s_ProfileManager.clear();
}

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
        qDebug() << "[QuickCutConsole::loadProfiles] - Failed to load "
                 << QString::fromStdString(s_ProfileManager.getConfigFilePath()) << " file.";
        return false;
    }

    s_Profile = s_ProfileManager.getActiveProfile();

    return true;
}

void QuickCutConsole::executeProcess(const std::string & process,
                                     const std::string & arguments)
{
    // QProc won't expand environment variable strings.
    // Invoking using the user console will allow for expanded string to work as expected.
#if defined(Q_OS_WIN)
    QString command   = "cmd /c start \"\" \"" + QString::fromStdString(process) + "\"";
    QString extension = ".cmd";
#elif defined(Q_OS_UNIX)
    QString command   = "sh -c '" + QString::fromStdString(process) + "'";
    QString extension = ".sh";
#endif

    QStringList argsTmp = QString::fromStdString(arguments).trimmed().split(",");
    for (auto && arg : argsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        command += " " + argTrimmed;
    }
    qDebug() << "[QuickCutConsole::executeProcess] - Execute Command: " << command;

    // Writing as script temporary to disk to avoid any white spaces issues
    // that QProcess doesn't handle very well...
    QString filePath = applicationDirPath() + "/tempCmd" + extension;
    QFile   scriptFile(filePath);
    scriptFile.open(QIODevice::ReadWrite);
    QTextStream ts(&scriptFile);
    ts << command;
    scriptFile.close();
    QProcess::execute(filePath, QStringList());
    scriptFile.remove();
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
