
#include "QuickCutPCH.h"
#include "QuickCutServiceUnix.h"

QuickCutServiceUnix::QuickCutServiceUnix(int argc, char * argv[])
    : QuickCutService(argc, argv)
{
}

QuickCutServiceUnix::~QuickCutServiceUnix()
{
    if (isProcessRunning(QUICKCUTCONSOLE_BIN))
        QProcess::execute("killall", QStringList("-9 " QUICKCUTCONSOLE_BIN));
}

void QuickCutServiceUnix::start()
{
    QuickCutService::start();

    QString process = QCoreApplication::applicationDirPath() + QUICKCUTCONSOLE_BIN;
    QProcess::execute(process, QStringList());
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
    if (isProcessRunning(QUICKCUTCONSOLE_BIN))
        QProcess::execute("killall", QStringList("-9 " QUICKCUTCONSOLE_BIN));
}

bool QuickCutServiceUnix::isProcessRunning(const QString & process)
{
    Q_UNUSED(process);
    return false;
}
