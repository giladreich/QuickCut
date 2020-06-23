
#include "pch.h"
#include "UpdatesWindow.h"

UpdatesWindow::UpdatesWindow(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::UpdatesWindow())
{
    ui->setupUi(this);
}

UpdatesWindow::~UpdatesWindow() {}
