
#pragma once

#include <QDialog>
#include "ui_UpdatesView.h"

class UpdatesView : public QDialog
{
    Q_OBJECT

public:
    UpdatesView(QWidget * parent = Q_NULLPTR);
    ~UpdatesView();

private:
    Ui::UpdatesView * ui;
};
