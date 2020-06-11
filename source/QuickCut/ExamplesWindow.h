
#pragma once

#include <QDialog>
#include "ui_ExamplesWindow.h"


class ExamplesWindow : public QDialog
{
    Q_OBJECT

public:
    ExamplesWindow(QWidget * parent = Q_NULLPTR);
    ~ExamplesWindow();

private:
    Ui::ExamplesWindow * ui;
};
