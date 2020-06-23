
#include "pch.h"
#include <QtCore/QCoreApplication>
#include "Utils/QSingleApplication/QSingleInstance.h"

#if defined(Q_OS_WIN)
#    include "QuickCutConsoleWindows.h"
#elif defined(Q_OS_UNIX)
#    include "QuickCutConsoleUnix.h"
#endif

int main(int argc, char * argv[])
{
    QSingleInstance instance("abbebe64-b962-42e7-8367-1c800819883b");
    if (!instance.tryToRun()) { return 0; }

    QCoreApplication::setApplicationName(QUICKCUTCONSOLE_NAME);
    QCoreApplication::setApplicationVersion(QUICKCUT_VERSION);

#if defined(Q_OS_WIN)
    QuickCutConsoleWindows qc(argc, argv);
#elif defined(Q_OS_UNIX)
    QuickCutConsoleUnix qc(argc, argv);
#endif

    qc.start();

    return qc.exec();
}
