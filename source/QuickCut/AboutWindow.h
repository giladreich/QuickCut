
#pragma once

#include <QDialog>
#include "ui_AboutWindow.h"


class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    AboutWindow(QWidget * parent = Q_NULLPTR);
    ~AboutWindow();

private:
    Ui::AboutWindow * ui;
};
