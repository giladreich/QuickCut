
#include "ActionEditWindow.h"
#include "Action.h"
#include "QShortcutInput.h"

#include <QStandardPaths>
#include <QFileDialog>
//#include <QKeySequenceEdit>

ActionEditWindow::ActionEditWindow(QWidget * parent, eEditMode editMode)
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
    ui->cbxType->addItem("Start Application");
}

void ActionEditWindow::fillEntries()
{
    ui->tbxName->setText(QString::fromStdString(m_Action->getName()));

    int typeIndex = static_cast<int>(m_Action->getType());
    ui->cbxType->setCurrentIndex(typeIndex);
    onTypeSelChange(typeIndex);

    // ui->tbxSrcKey->setKeySequence(QKeySequence::fromString(QString::fromStdString(m_Action->getSrcKey())));
    // ui->tbxDstKey->setKeySequence(QKeySequence::fromString(QString::fromStdString(m_Action->getDstKey())));
    ui->tbxSrcKey->setText(QString::fromStdString(m_Action->getSrcKey()));
    ui->tbxDstKey->setText(QString::fromStdString(m_Action->getDstKey()));
    ui->tbxAppPath->setText(QString::fromStdString(m_Action->getAppPath()));
    ui->tbxAppArgs->setText(QString::fromStdString(m_Action->getAppArgs()));
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

eEditMode ActionEditWindow::getEditMode()
{
    return m_EditMode;
}

void ActionEditWindow::onTypeSelChange(int index)
{
    if (index < 0 || index > ActionAppStart) return;

    eActionType type = static_cast<eActionType>(index);
    if (type == ActionKeyMap)
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
    else if (type == ActionAppStart)
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

    eActionType type = static_cast<eActionType>(ui->cbxType->currentIndex());

    m_Action->setName(ui->tbxName->text().toStdString());
    m_Action->setType(type);
    m_Action->setSrcKey(ui->tbxSrcKey->text().toStdString());
    // m_Action->setSrcKey(ui->tbxSrcKey->keySequence().toString().toStdString());
    // if (type == ActionKeyMap) {
    // m_Action->setDstKey(ui->tbxDstKey->keySequence().toString().toStdString()); }
    if (type == ActionKeyMap) { m_Action->setDstKey(ui->tbxDstKey->text().toStdString()); }
    else if (type == ActionAppStart)
    {
        m_Action->setAppPath(ui->tbxAppPath->text().toStdString());
        m_Action->setAppArgs(ui->tbxAppArgs->text().toStdString());
    }

    if (m_EditMode == ActionCreate)
        emit onCreated(m_Action);
    else
        emit onSaved();

    close();
}
