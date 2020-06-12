
#include "AboutWindow.h"

AboutWindow::AboutWindow(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::AboutWindow())
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->btnClose, &QPushButton::clicked, this, [this] { close(); });
}

AboutWindow::~AboutWindow() {}
