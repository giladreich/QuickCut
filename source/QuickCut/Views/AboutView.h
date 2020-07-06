
#pragma once

#include <QDialog>
#include "ui_AboutView.h"

class AboutView : public QDialog
{
    Q_OBJECT

public:
    AboutView(QWidget * parent = Q_NULLPTR);
    ~AboutView();

private:
    std::unique_ptr<Ui::AboutView> ui;
};
