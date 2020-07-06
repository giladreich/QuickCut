
#include "QuickCutShared/QuickCutPCH.h"
#include "UpdatesView.h"

UpdatesView::UpdatesView(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::UpdatesView>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

UpdatesView::~UpdatesView() = default;
