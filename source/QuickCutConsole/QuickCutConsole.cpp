
#include "QuickCutConsole.h"
#include "pch.h"
#include "Profile.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>

QuickCutConsole * QuickCutConsole::s_Instance = nullptr;

std::unique_ptr<Profile> QuickCutConsole::s_Profile = nullptr;
String                   QuickCutConsole::s_ProfilesPath;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
{
    if (!s_Instance) s_Instance = this;
}

QuickCutConsole::~QuickCutConsole() {}

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
    QFileInfo fi(applicationDirPath() + "/Config/profiles.json");
    if (!fi.exists())
    {
        qDebug() << "[QuickCutConsole::loadProfiles] - Profiles file not found: "
                 << fi.filePath();
        return false;
    }

    s_ProfilesPath = fi.filePath().toStdString();

    JSON rootJson;
    bpt::read_json(s_ProfilesPath, rootJson);
    String szActiveProfile = rootJson.get<String>("activeProfile", "");

    JSON profilesJson = rootJson.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        String profileId = profileJson.second.get<String>("id", "");
        if (profileId != szActiveProfile) continue;

        String profileName  = profileJson.second.get<String>("name", "");
        String lastModified = profileJson.second.get<String>("lastModified", "");
        int    actionsCount = profileJson.second.get<int>("actionsCount", 0);

        s_Profile = std::make_unique<Profile>(profileId, profileName, lastModified);
        s_Profile->setActionsCapacity(actionsCount);

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

            s_Profile->addAction(new Action(actionId, actionName, Action::getType(actionType),
                                            srcKey, dstKey, appPath, appArgs, createdDate));
        }

        break;
    }

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
    QFile   file(filePath);
    file.open(QIODevice::ReadWrite);
    QTextStream ts(&file);
    ts << command;
    file.close();
    QProcess::execute(filePath, QStringList());
    file.remove();
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
