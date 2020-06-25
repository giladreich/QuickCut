
#include "QuickCutPCH.h"
#include "UpdatesView.h"

UpdatesView::UpdatesView(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::UpdatesView())
{
    ui->setupUi(this);
}

UpdatesView::~UpdatesView() {}
