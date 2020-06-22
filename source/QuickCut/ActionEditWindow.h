
#pragma once

#include <QDialog>
#include "ui_ActionEditWindow.h"

class Action;
class QShortcutInput;

enum EditMode
{
    ActionEdit   = 1,
    ActionCreate = 2
};

class ActionEditWindow : public QDialog
{
    Q_OBJECT

private:
    ActionEditWindow(QWidget * parent, EditMode EditMode);

public:
    ActionEditWindow(QWidget * parent);
    ActionEditWindow(QWidget * parent, Action * action);
    ~ActionEditWindow();

    void fillActionTypes();
    void fillEntries();
    void connectSlots();

    EditMode getEditMode();

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
    Ui::ActionEditWindow * ui;

    Action * m_Action;

    EditMode m_EditMode;
};
