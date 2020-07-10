
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutServiceUnix.h"

QuickCutServiceUnix::QuickCutServiceUnix(int argc, char * argv[])
    : QuickCutService(argc, argv)
{
}

QuickCutServiceUnix::~QuickCutServiceUnix()
{
    killHookIfRunning();
}

void QuickCutServiceUnix::start()
{
    QuickCutService::start();

    startHook();
}

void QuickCutServiceUnix::stop()
{
    QuickCutService::stop();

    killHookIfRunning();
}

void QuickCutServiceUnix::pause()
{
    QuickCutService::pause();

    stop();
}

void QuickCutServiceUnix::resume()
{
    QuickCutService::resume();

    start();
}

bool QuickCutServiceUnix::startHook()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString process = QDir(appPath).filePath(QUICKCUTCONSOLE_BIN);
    QProcess::execute(process, QStringList());
    return true;
}

bool QuickCutServiceUnix::isHookRunning()
{
    // TODO(Gilad): Implement.
    return true;
}

void QuickCutServiceUnix::killHookIfRunning()
{
    if (isHookRunning()) QProcess::execute("killall", QStringList("-9 " QUICKCUTCONSOLE_BIN));
}