
#include "pch.h"
#include "ExamplesWindow.h"

ExamplesWindow::ExamplesWindow(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::ExamplesWindow())
{
    ui->setupUi(this);
}

ExamplesWindow::~ExamplesWindow() {}
