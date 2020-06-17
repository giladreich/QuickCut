
#pragma once

#include <QDialog>
#include "ui_ActionEditWindow.h"

class Action;
class QShortcutInput;

enum eEditMode
{
    ActionEdit   = 1,
    ActionCreate = 2
};

class ActionEditWindow : public QDialog
{
    Q_OBJECT

private:
    ActionEditWindow(QWidget * parent, eEditMode eEditMode);

public:
    ActionEditWindow(QWidget * parent);
    ActionEditWindow(QWidget * parent, Action * pAction);
    ~ActionEditWindow();

    void fillActionTypes();
    void fillEntries();
    void connectSlots();

    eEditMode getEditMode();

signals:
    void onSaved();
    void onCreated(Action * pAction);
    void onClosed();

public slots:

    void onTypeSelChange(int iIndex);

    //void onBtnKeyPlay(QKeySequenceEdit * pInput, QPushButton * pBtn);
    void onBtnKeyPlay(QShortcutInput * pInput, QPushButton * pBtn);
    void onBtnFilePicker();
    void onBtnCancel();
    void onBtnSave();

private:
    Ui::ActionEditWindow * ui;

    Action * m_pAction;

    eEditMode m_eEditMode;
};
