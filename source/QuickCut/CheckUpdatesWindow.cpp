
#include "CheckUpdatesWindow.h"

CheckUpdatesWindow::CheckUpdatesWindow(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::CheckUpdatesWindow())
{
    ui->setupUi(this);
}

CheckUpdatesWindow::~CheckUpdatesWindow() {}
