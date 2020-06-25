
#include "QuickCutPCH.h"
#include "ActionView.h"
#include "Models/Action.h"
#include "Widgets/QShortcutInput.h"

#include <QFileDialog>
//#include <QKeySequenceEdit>

ActionView::ActionView(QWidget * parent, EditMode editMode)
    : QDialog(parent)
    , ui(new Ui::ActionView())
    , m_EditMode(editMode)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    fillActionTypes();
    connectSlots();
}

ActionView::ActionView(QWidget * parent)
    : ActionView(parent, ActionCreate)
{
    m_Action = new Action();
    ui->btnSave->setText("Create");
    onTypeSelChange(0);
}

ActionView::ActionView(QWidget * parent, Action * action)
    : ActionView(parent, ActionEdit)
{
    m_Action = action;
    ui->btnSave->setText("Save");

    fillEntries();
}

ActionView::~ActionView() {}

void ActionView::fillActionTypes()
{
    ui->cbxType->addItem("Key Mapping");
    ui->cbxType->addItem("Application Launch");
    ui->cbxType->addItem("Directory Launch");
}

void ActionView::fillEntries()
{
    int typeIndex = static_cast<int>(m_Action->getType());

    ui->tbxName->setText(m_Action->getName());
    ui->cbxType->setCurrentIndex(typeIndex);
    // ui->tbxSrcKey->setKeySequence(QKeySequence::fromString(m_Action->getSrcKey()));
    ui->tbxSrcKey->setText(m_Action->getSrcKey());
    onTypeSelChange(typeIndex);
}

void ActionView::connectSlots()
{
    connect(ui->cbxType, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &ActionView::onTypeSelChange);
    connect(ui->btnSrcKeyPlay, &QPushButton::clicked, this,
            [this] { onBtnKeyPlay(ui->tbxSrcKey, ui->btnSrcKeyPlay); });
    connect(ui->btnDstKeyPlay, &QPushButton::clicked, this,
            [this] { onBtnKeyPlay(ui->tbxDstKey, ui->btnDstKeyPlay); });
    connect(ui->btnPicker, &QPushButton::clicked, this, &ActionView::onBtnFilePicker);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ActionView::onBtnCancel);
    connect(ui->btnSave, &QPushButton::clicked, this, &ActionView::onBtnSave);
}

void ActionView::updateVisibility(Action::ActionType type)
{
    switch (type)
    {
        case Action::ActionKeyMap:
        default:
        {
            ui->lblDstKey->setVisible(true);
            ui->tbxDstKey->setVisible(true);
            ui->btnDstKeyPlay->setVisible(true);
            ui->lblAppStart->setVisible(false);
            ui->tbxTargetPath->setVisible(false);
            ui->btnPicker->setVisible(false);
            ui->lblAppArgs->setVisible(false);
            ui->tbxAppArgs->setVisible(false);
            break;
        }
        case Action::ActionAppLaunch:
        {
            ui->lblDstKey->setVisible(false);
            ui->tbxDstKey->setVisible(false);
            ui->btnDstKeyPlay->setVisible(false);
            ui->lblAppStart->setVisible(true);
            ui->tbxTargetPath->setVisible(true);
            ui->btnPicker->setVisible(true);
            ui->lblAppArgs->setVisible(true);
            ui->tbxAppArgs->setVisible(true);
            break;
        }
        case Action::ActionDirLaunch:
        {
            ui->lblDstKey->setVisible(false);
            ui->tbxDstKey->setVisible(false);
            ui->btnDstKeyPlay->setVisible(false);
            ui->lblAppStart->setVisible(true);
            ui->tbxTargetPath->setVisible(true);
            ui->btnPicker->setVisible(true);
            ui->lblAppArgs->setVisible(false);
            ui->tbxAppArgs->setVisible(false);
            break;
        }
    }
}

EditMode ActionView::getEditMode()
{
    return m_EditMode;
}

void ActionView::onTypeSelChange(int index)
{
    Action::ActionType type = static_cast<Action::ActionType>(index);
    m_Action->setType(type);
    updateVisibility(type);
    switch (type)
    {
        case Action::ActionKeyMap:
        default:
        {
            // ui->tbxDstKey->setKeySequence(QKeySequence::fromString(m_Action->getDstKey()));
            ui->tbxDstKey->setText(m_Action->getDstKey());
            ui->tbxTargetPath->clear();
            ui->tbxAppArgs->clear();
            break;
        }
        case Action::ActionAppLaunch:
        {
            ui->tbxDstKey->clear();
            ui->tbxTargetPath->setText(m_Action->getTargetPath());
            ui->tbxAppArgs->setText(m_Action->getAppArgs());
            break;
        }
        case Action::ActionDirLaunch:
        {
            ui->tbxDstKey->clear();
            ui->tbxTargetPath->setText(m_Action->getTargetPath());
            ui->tbxAppArgs->clear();
            break;
        }
    }
}

// void ActionView::onBtnKeyPlay(QKeySequenceEdit * input, QPushButton * button)
void ActionView::onBtnKeyPlay(QShortcutInput * input, QPushButton * button)
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

void ActionView::onBtnFilePicker()
{
    QString homeDir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();

    QString filePath;
    if (m_Action->getType() == Action::ActionAppLaunch)
        filePath =
            QFileDialog::getOpenFileName(this, "Get Target File", homeDir, "All files (*.*)");
    else if (m_Action->getType() == Action::ActionDirLaunch)
        filePath = QFileDialog::getExistingDirectory(this, "Get Target Directory", homeDir);

    if (filePath.isEmpty()) return;

    ui->tbxTargetPath->setText(filePath);
}

void ActionView::onBtnCancel()
{
    if (m_EditMode == ActionCreate)
    {
        delete m_Action;
        m_Action = nullptr;
    }

    close();
}

void ActionView::onBtnSave()
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
    m_Action->setDstKey(ui->tbxDstKey->text());
    m_Action->setTargetPath(ui->tbxTargetPath->text());
    m_Action->setAppArgs(ui->tbxAppArgs->text());

    if (m_EditMode == ActionCreate)
        emit onCreated(m_Action);
    else
        emit onSaved();

    close();
}
