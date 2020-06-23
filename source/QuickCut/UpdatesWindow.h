
#pragma once

#include <QDialog>
#include "ui_UpdatesWindow.h"

class UpdatesWindow : public QDialog
{
    Q_OBJECT

public:
    UpdatesWindow(QWidget * parent = Q_NULLPTR);
    ~UpdatesWindow();

private:
    Ui::UpdatesWindow * ui;
};
