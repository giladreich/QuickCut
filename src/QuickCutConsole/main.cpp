
#include <QtCore/QCoreApplication>
#include <QSingleApplication/QSingleInstance.h>


#ifdef Q_OS_WIN
#include "QuickCutConsoleWindows.h"
#elif Q_OS_UNIX
#include "QuickCutConsoleUnix.h"
#endif


int main(int argc, char *argv[])
{
    QSingleInstance instance("abbebe64-b962-42e7-8367-1c800819883b");
    if (!instance.tryToRun())
    {
        return 0;
    }

#ifdef Q_OS_WIN
    QuickCutConsoleWindows qc(argc, argv);
#elif Q_OS_UNIX
    QuickCutConsoleUnix qc(argc, argv);
#endif

    qc.start();

    return qc.exec();
}
