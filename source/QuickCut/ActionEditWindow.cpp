
#include "ActionEditWindow.h"
#include "Action.h"
#include "QShortcutInput.h"

#include <QStandardPaths>
#include <QFileDialog>
//#include <QKeySequenceEdit>


ActionEditWindow::ActionEditWindow(QWidget * parent, eEditMode eEditMode)
    : QDialog(parent)
    , ui(new Ui::ActionEditWindow())
    , m_eEditMode(eEditMode)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    fillActionTypes();
    connectSlots();
}

ActionEditWindow::ActionEditWindow(QWidget * parent)
    : ActionEditWindow(parent, ActionCreate)
{
    m_pAction = new Action();
    ui->btnSave->setText("Create");
    onTypeSelChange(0);
}

ActionEditWindow::ActionEditWindow(QWidget * parent, Action * pAction)
    : ActionEditWindow(parent, ActionEdit)
{
    m_pAction = pAction;
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
    ui->tbxName->setText(QString::fromStdString(m_pAction->getName()));

    int iTypeIndex = static_cast<int>(m_pAction->getType());
    ui->cbxType->setCurrentIndex(iTypeIndex);
    onTypeSelChange(iTypeIndex);

    //ui->tbxSrcKey->setKeySequence(QKeySequence::fromString(QString::fromStdString(m_pAction->getSrcKey())));
    //ui->tbxDstKey->setKeySequence(QKeySequence::fromString(QString::fromStdString(m_pAction->getDstKey())));
    ui->tbxSrcKey->setText(QString::fromStdString(m_pAction->getSrcKey()));
    ui->tbxDstKey->setText(QString::fromStdString(m_pAction->getDstKey()));
    ui->tbxAppPath->setText(QString::fromStdString(m_pAction->getAppPath()));
    ui->tbxAppArgs->setText(QString::fromStdString(m_pAction->getAppArgs()));
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
    return m_eEditMode;
}

void ActionEditWindow::onTypeSelChange(int iIndex)
{
    if (iIndex < 0 || iIndex > ActionAppStart) return;

    eActionType eType = static_cast<eActionType>(iIndex);
    if (eType == ActionKeyMap)
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
    else if (eType == ActionAppStart)
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

//void ActionEditWindow::onBtnKeyPlay(QKeySequenceEdit * pInput, QPushButton * pBtn)
void ActionEditWindow::onBtnKeyPlay(QShortcutInput * pInput, QPushButton * pBtn)
{
    if (pInput->isEnabled())
    {
        for (auto && widget : findChildren<QWidget *>()) widget->setEnabled(true);

        pBtn->setProperty("background-image", "play");
        pBtn->style()->polish(pBtn);
        pBtn->setFocus();
        pBtn->setEnabled(true);

        ui->tbxSrcKey->setEnabled(false);
        ui->tbxDstKey->setEnabled(false);
    }
    else
    {
        for (auto&& widget : findChildren<QWidget*>()) widget->setEnabled(false);

        pBtn->setProperty("background-image", "stop");
        pBtn->style()->polish(pBtn);
        pBtn->setEnabled(true);
        pInput->setEnabled(true);
        pInput->setFocus();
    }
}

void ActionEditWindow::onBtnFilePicker()
{
    QString szHome = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    QString szFilePath =
        QFileDialog::getOpenFileName(this, tr("Open File"), szHome, tr("All files (*.*)"));
    if (szFilePath.isEmpty()) return;

    ui->tbxAppPath->setText(szFilePath);
}

void ActionEditWindow::onBtnCancel()
{
    if (m_eEditMode == ActionCreate) delete m_pAction;

    close();
}

void ActionEditWindow::onBtnSave()
{
    if (ui->tbxName->text().isEmpty())
    {
        ui->tbxName->setFocus();
        return;
    }

    m_pAction->reset();

    eActionType eType = static_cast<eActionType>(ui->cbxType->currentIndex());

    m_pAction->setName(ui->tbxName->text().toStdString());
    m_pAction->setType(eType);
    m_pAction->setSrcKey(ui->tbxSrcKey->text().toStdString());
    //m_pAction->setSrcKey(ui->tbxSrcKey->keySequence().toString().toStdString());
    //if (eType == ActionKeyMap) { m_pAction->setDstKey(ui->tbxDstKey->keySequence().toString().toStdString()); }
    if (eType == ActionKeyMap) { m_pAction->setDstKey(ui->tbxDstKey->text().toStdString()); }
    else if (eType == ActionAppStart)
    {
        m_pAction->setAppPath(ui->tbxAppPath->text().toStdString());
        m_pAction->setAppArgs(ui->tbxAppArgs->text().toStdString());
    }

    if (m_eEditMode == ActionCreate)
        emit onCreated(m_pAction);
    else
        emit onSaved();

    close();
}
