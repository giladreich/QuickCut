
#pragma once

#include <QDialog>
#include "ui_ExamplesView.h"

class ExamplesView : public QDialog
{
    Q_OBJECT

public:
    ExamplesView(QWidget * parent = Q_NULLPTR);
    ~ExamplesView();

private:
    Ui::ExamplesView * ui;
};
