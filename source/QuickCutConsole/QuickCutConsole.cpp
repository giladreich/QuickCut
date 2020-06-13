
#include "QuickCutConsole.h"
#include "pch.h"
#include "Profile.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>

QuickCutConsole * QuickCutConsole::s_pInstance = nullptr;

std::unique_ptr<Profile> QuickCutConsole::s_pProfile = nullptr;
String                   QuickCutConsole::s_qszProfilesPath;

QuickCutConsole::QuickCutConsole(int argc, char * argv[])
    : QCoreApplication(argc, argv)
{
    if (!s_pInstance) s_pInstance = this;
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
    QFileInfo fiProfiles(applicationDirPath() + "/Config/profiles.json");
    if (!fiProfiles.exists())
    {
        qDebug() << "[QuickCutConsole::loadProfiles] - Profiles file not found: "
                 << fiProfiles.filePath();
        return false;
    }

    s_qszProfilesPath = fiProfiles.filePath().toStdString();

    JSON rootJson;
    bpt::read_json(s_qszProfilesPath, rootJson);
    String szActiveProfile = rootJson.get<String>("activeProfile", "");

    JSON profilesJson = rootJson.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        String profileId = profileJson.second.get<String>("id", "");
        if (profileId != szActiveProfile) continue;

        String profileName  = profileJson.second.get<String>("name", "");
        String lastModified = profileJson.second.get<String>("lastModified", "");
        int    actionsCount = profileJson.second.get<int>("actionsCount", 0);

        s_pProfile = std::make_unique<Profile>(profileId, profileName, lastModified);
        s_pProfile->setActionsCapacity(actionsCount);

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

            s_pProfile->addAction(new Action(actionId, actionName, Action::getType(actionType),
                                             srcKey, dstKey, appPath, appArgs, createdDate));
        }

        break;
    }

    return true;
}

void QuickCutConsole::executeProcess(const std::string & szProc, const std::string & szArgs)
{
    // QProc won't expand environment variable strings.
    // Invoking using the user console will allow for expanded string to work as expected.
#if defined(Q_OS_WIN)
    QString szCommand = "cmd /c start \"\" \"" + QString::fromStdString(szProc) + "\"";
    QString szExt     = ".cmd";
#elif defined(Q_OS_UNIX)
    QString szCommand = "sh -c '" + QString::fromStdString(szProc) + "'";
    QString szExt     = ".sh";
#endif

    QStringList qArgsTmp = QString::fromStdString(szArgs).trimmed().split(",");
    for (auto && arg : qArgsTmp)
    {
        QString argTrimmed = arg.trimmed();
        if (argTrimmed.isEmpty()) continue;

        szCommand += " " + argTrimmed;
    }
    qDebug() << "[QuickCutConsole::executeProcess] - Execute Command: " << szCommand;

    // Writing as script temporary to disk to avoid any white spaces issues
    // that QProcess doesn't handle very well...
    QString szFilePath = applicationDirPath() + "/tempCmd" + szExt;
    QFile   file(szFilePath);
    file.open(QIODevice::ReadWrite);
    QTextStream ts(&file);
    ts << szCommand;
    file.close();
    QProcess::execute(szFilePath, QStringList());
    file.remove();
}

void QuickCutConsole::log(const QString & szFilePath, const QString & szMessage)
{
    QFile file(szFilePath);
    file.open(QFile::WriteOnly);
    QTextStream text(&file);
    text << szMessage;
    file.flush();
    file.close();
}
