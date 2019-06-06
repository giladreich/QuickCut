
#pragma once

#include <QDialog>
#include "ui_CheckUpdatesWindow.h"


class CheckUpdatesWindow : public QDialog
{
    Q_OBJECT

public:
    CheckUpdatesWindow(QWidget * parent = Q_NULLPTR);
    ~CheckUpdatesWindow();

private:
    Ui::CheckUpdatesWindow * ui;
};
