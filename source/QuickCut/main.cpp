
#include "pch.h"
#include <QtWidgets/QApplication>
#include "MainWindow.h"
#include "Utils/QSingleApplication/QSingleInstance.h"

int main(int argc, char * argv[])
{
    QSingleInstance instance("8c06d447-da32-447c-adfd-2d227605ae4b");
    if (!instance.tryToRun()) { return 0; }

    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
