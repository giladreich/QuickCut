
#include "QuickCutShared/QuickCutPCH.h"
#include "AboutView.h"

AboutView::AboutView(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::AboutView>())
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->btnClose, &QPushButton::clicked, this, [this] { close(); });
}

AboutView::~AboutView() = default;
