
#include "QuickCutPCH.h"
#include "QuickCutService.h"

QuickCutService::QuickCutService(int argc, char * argv[])
    : QtService<QCoreApplication>(argc, argv, QUICKCUTSERVICE_NAME)
{
    setServiceDescription(
        "QuickCutService is responsible for making sure QuickCutConsole background process is "
        "running. The user-defined shortcut keys will only be activated, if QuickCutConsole "
        "background process is running. User defined shortcut keys are specified using the "
        "QuickCut - GUI.");

    setServiceFlags(QtServiceBase::CanBeSuspended);
    setStartupType(QtServiceController::StartupType::AutoStartup);
}

QuickCutService::~QuickCutService() {}

void QuickCutService::start() {}

void QuickCutService::pause()
{
    stop();
}

void QuickCutService::resume()
{
    start();
}

void QuickCutService::stop() {}

bool QuickCutService::isProcessRunning(const QString & process)
{
    return false;
}

void QuickCutService::log(const QString & filePath, const QString & text)
{
    QFile file(filePath);
    file.open(QFile::WriteOnly | QFile::Append);
    QTextStream ts(&file);
    ts << '\n' << text;
    file.flush();
    file.close();
}
