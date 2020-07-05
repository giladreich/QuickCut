
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
    Ui::AboutView * ui;
};
