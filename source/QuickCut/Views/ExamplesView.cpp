
#include "QuickCutShared/QuickCutPCH.h"
#include "ExamplesView.h"

ExamplesView::ExamplesView(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::ExamplesView())
{
    ui->setupUi(this);
}

ExamplesView::~ExamplesView() {}
