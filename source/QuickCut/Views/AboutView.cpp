
#include "QuickCutShared/QuickCutPCH.h"
#include "AboutView.h"

AboutView::AboutView(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::AboutView())
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->btnClose, &QPushButton::clicked, this, [this] { close(); });
}

AboutView::~AboutView() {}
