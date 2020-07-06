
#include "QuickCutShared/QuickCutPCH.h"
#include "AboutView.h"

AboutView::AboutView(QWidget * parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::AboutView>())
{
    ui->setupUi(this);
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) |
                   Qt::MSWindowsFixedSizeDialogHint);

    QString version = ui->lblVersion->text();
    version         = version.replace("XXVERSIONXX", QUICKCUT_VERSION);
    ui->lblVersion->setText(version);

    QString year = ui->lblCopyRight->text();
    year         = year.replace("XXYEARXX", QString::number(QDate::currentDate().year()));
    ui->lblCopyRight->setText(year);

    connect(ui->btnClose, &QPushButton::clicked, this, [this] { close(); });
}

AboutView::~AboutView() = default;
