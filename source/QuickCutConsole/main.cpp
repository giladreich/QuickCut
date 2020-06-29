
#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutShared/Utils/QSingleApplication/QSingleInstance.h"

#if defined(Q_OS_WIN)
#    include "QuickCutConsoleWindows.h"
#elif defined(Q_OS_UNIX)
#    include "QuickCutConsoleUnix.h"
#endif

#include <QtCore/QCoreApplication>

int main(int argc, char * argv[])
{
    QSingleInstance instance("abbebe64-b962-42e7-8367-1c800819883b");
    if (!instance.tryToRun()) { return 0; }

    QCoreApplication::setApplicationName(QUICKCUTCONSOLE_NAME);
    QCoreApplication::setApplicationVersion(QUICKCUT_VERSION);

    std::unique_ptr<QuickCutConsole> qc;
#if defined(Q_OS_WIN)
    qc = std::make_unique<QuickCutConsoleWindows>(argc, argv);
#elif defined(Q_OS_UNIX)
    qc = std::make_unique<QuickCutConsoleUnix>(argc, argv);
#endif
    qc->start();

    return qc->exec();
}
