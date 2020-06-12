
#include "QuickCutServiceUnix.h"
#include "pch.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

QuickCutServiceUnix::QuickCutServiceUnix(int argc, char * argv[])
    : QuickCutService(argc, argv)
{
}

QuickCutServiceUnix::~QuickCutServiceUnix()
{
    if (isProcessRunning("QuickCutConsole")) QProcess::execute("killall -9 QuickCutConsole");
}

void QuickCutServiceUnix::start()
{
    QuickCutService::start();

    QString proc = QCoreApplication::applicationDirPath() + "/QuickCutConsole";
    QProcess::execute(proc);
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

void QuickCutServiceUnix::stop()
{
    QuickCutService::stop();
    if (isProcessRunning("QuickCutConsole")) QProcess::execute("killall -9 QuickCutConsole");
}

bool QuickCutServiceUnix::isProcessRunning(const QString & szProc)
{
    return false;
}
