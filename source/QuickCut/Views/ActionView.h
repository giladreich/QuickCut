
#pragma once

#include <QDialog>
#include "Models/Action.h"
#include "ui_ActionView.h"

class QShortcutInput;

enum EditMode
{
    ActionEdit   = 1,
    ActionCreate = 2
};

class ActionView : public QDialog
{
    Q_OBJECT

private:
    ActionView(QWidget * parent, EditMode EditMode);

public:
    ActionView(QWidget * parent);
    ActionView(QWidget * parent, Action * action);
    ~ActionView();

    EditMode getEditMode();

private:
    void fillActionTypes();
    void fillEntries();
    void connectSlots();
    void updateVisibility(Action::ActionType type);

signals:
    void onSaved();
    void onCreated(Action * action);
    void onClosed();

public slots:

    void onTypeSelChange(int index);

    // void onBtnKeyPlay(QKeySequenceEdit * input, QPushButton * pBtn);
    void onBtnKeyPlay(QShortcutInput * input, QPushButton * button);
    void onBtnFilePicker();
    void onBtnCancel();
    void onBtnSave();

private:
    Ui::ActionView * ui;

    Action * m_Action;

    EditMode m_EditMode;
};
