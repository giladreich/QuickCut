
#include "pch.h"
#include "ActionEditWindow.h"
#include "Models/Action.h"
#include "Widgets/QShortcutInput.h"

#include <QFileDialog>
//#include <QKeySequenceEdit>

ActionEditWindow::ActionEditWindow(QWidget * parent, EditMode editMode)
    : QDialog(parent)
    , ui(new Ui::ActionEditWindow())
    , m_EditMode(editMode)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    fillActionTypes();
    connectSlots();
}

ActionEditWindow::ActionEditWindow(QWidget * parent)
    : ActionEditWindow(parent, ActionCreate)
{
    m_Action = new Action();
    ui->btnSave->setText("Create");
    onTypeSelChange(0);
}

ActionEditWindow::ActionEditWindow(QWidget * parent, Action * action)
    : ActionEditWindow(parent, ActionEdit)
{
    m_Action = action;
    ui->btnSave->setText("Save");

    fillEntries();
}

ActionEditWindow::~ActionEditWindow() {}

void ActionEditWindow::fillActionTypes()
{
    ui->cbxType->addItem("Key Mapping");
    ui->cbxType->addItem("Application Launch");
}

void ActionEditWindow::fillEntries()
{
    ui->tbxName->setText(m_Action->getName());

    int typeIndex = static_cast<int>(m_Action->getType());
    ui->cbxType->setCurrentIndex(typeIndex);
    onTypeSelChange(typeIndex);

    // ui->tbxSrcKey->setKeySequence(QKeySequence::fromString(m_Action->getSrcKey()));
    // ui->tbxDstKey->setKeySequence(QKeySequence::fromString(m_Action->getDstKey()));
    ui->tbxSrcKey->setText(m_Action->getSrcKey());
    ui->tbxDstKey->setText(m_Action->getDstKey());
    ui->tbxAppPath->setText(m_Action->getAppPath());
    ui->tbxAppArgs->setText(m_Action->getAppArgs());
}

void ActionEditWindow::connectSlots()
{
    connect(ui->cbxType, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &ActionEditWindow::onTypeSelChange);
    connect(ui->btnSrcKeyPlay, &QPushButton::clicked, this,
            [this] { onBtnKeyPlay(ui->tbxSrcKey, ui->btnSrcKeyPlay); });
    connect(ui->btnDstKeyPlay, &QPushButton::clicked, this,
            [this] { onBtnKeyPlay(ui->tbxDstKey, ui->btnDstKeyPlay); });
    connect(ui->btnFilePicker, &QPushButton::clicked, this,
            &ActionEditWindow::onBtnFilePicker);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ActionEditWindow::onBtnCancel);
    connect(ui->btnSave, &QPushButton::clicked, this, &ActionEditWindow::onBtnSave);
}

EditMode ActionEditWindow::getEditMode()
{
    return m_EditMode;
}

void ActionEditWindow::onTypeSelChange(int index)
{
    if (index < 0 || index > Action::ActionAppLaunch) return;

    Action::ActionType type = static_cast<Action::ActionType>(index);
    if (type == Action::ActionKeyMap)
    {
        ui->lblDstKey->setVisible(true);
        ui->tbxDstKey->setVisible(true);
        ui->btnDstKeyPlay->setVisible(true);

        ui->lblAppStart->setVisible(false);
        ui->tbxAppPath->setVisible(false);
        ui->btnFilePicker->setVisible(false);
        ui->lblAppArgs->setVisible(false);
        ui->tbxAppArgs->setVisible(false);
    }
    else if (type == Action::ActionAppLaunch)
    {
        ui->lblDstKey->setVisible(false);
        ui->tbxDstKey->setVisible(false);
        ui->btnDstKeyPlay->setVisible(false);

        ui->lblAppStart->setVisible(true);
        ui->tbxAppPath->setVisible(true);
        ui->btnFilePicker->setVisible(true);
        ui->lblAppArgs->setVisible(true);
        ui->tbxAppArgs->setVisible(true);
    }
}

// void ActionEditWindow::onBtnKeyPlay(QKeySequenceEdit * input, QPushButton * button)
void ActionEditWindow::onBtnKeyPlay(QShortcutInput * input, QPushButton * button)
{
    if (input->isEnabled())
    {
        for (auto && widget : findChildren<QWidget *>()) widget->setEnabled(true);

        button->setProperty("background-image", "play");
        button->style()->polish(button);
        button->setFocus();
        button->setEnabled(true);

        ui->tbxSrcKey->setEnabled(false);
        ui->tbxDstKey->setEnabled(false);
    }
    else
    {
        for (auto && widget : findChildren<QWidget *>()) widget->setEnabled(false);

        button->setProperty("background-image", "stop");
        button->style()->polish(button);
        button->setEnabled(true);
        input->setEnabled(true);
        input->setFocus();
    }
}

void ActionEditWindow::onBtnFilePicker()
{
    QString homeDir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString filePath =
        QFileDialog::getOpenFileName(this, tr("Open File"), homeDir, tr("All files (*.*)"));
    if (filePath.isEmpty()) return;

    ui->tbxAppPath->setText(filePath);
}

void ActionEditWindow::onBtnCancel()
{
    if (m_EditMode == ActionCreate) delete m_Action;

    close();
}

void ActionEditWindow::onBtnSave()
{
    if (ui->tbxName->text().isEmpty())
    {
        ui->tbxName->setFocus();
        return;
    }

    m_Action->reset();

    Action::ActionType type = static_cast<Action::ActionType>(ui->cbxType->currentIndex());

    m_Action->setName(ui->tbxName->text());
    m_Action->setType(type);
    m_Action->setSrcKey(ui->tbxSrcKey->text());
    // m_Action->setSrcKey(ui->tbxSrcKey->keySequence().toString());
    // if (type == ActionKeyMap) {
    // m_Action->setDstKey(ui->tbxDstKey->keySequence().toString()); }
    if (type == Action::ActionKeyMap) { m_Action->setDstKey(ui->tbxDstKey->text()); }
    else if (type == Action::ActionAppLaunch)
    {
        m_Action->setAppPath(ui->tbxAppPath->text());
        m_Action->setAppArgs(ui->tbxAppArgs->text());
    }

    if (m_EditMode == ActionCreate)
        emit onCreated(m_Action);
    else
        emit onSaved();

    close();
}
