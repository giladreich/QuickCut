
#include "QuickCutShared/QuickCutPCH.h"
#include "ExamplesView.h"

ExamplesView::ExamplesView(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::ExamplesView>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ExamplesView::~ExamplesView() = default;
