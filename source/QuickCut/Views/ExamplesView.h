
#pragma once

#include <QDialog>
#include "ui_ExamplesView.h"

class ExamplesView : public QDialog
{
    Q_OBJECT

public:
    ExamplesView(QWidget * parent = nullptr);
    ~ExamplesView();

private:
    std::unique_ptr<Ui::ExamplesView> ui;
};
