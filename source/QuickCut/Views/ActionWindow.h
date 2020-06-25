
#pragma once

#include <QDialog>
#include "Models/Action.h"
#include "ui_ActionWindow.h"

class QShortcutInput;

enum EditMode
{
    ActionEdit   = 1,
    ActionCreate = 2
};

class ActionWindow : public QDialog
{
    Q_OBJECT

private:
    ActionWindow(QWidget * parent, EditMode EditMode);

public:
    ActionWindow(QWidget * parent);
    ActionWindow(QWidget * parent, Action * action);
    ~ActionWindow();

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
    Ui::ActionWindow * ui;

    Action * m_Action;

    EditMode m_EditMode;
};