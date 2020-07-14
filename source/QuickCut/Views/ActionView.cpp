
#include "QuickCutShared/QuickCutPCH.h"
#include "ActionView.h"

#include "Widgets/QShortcutInput.h"

#include "QuickCutShared/Models/Action.h"

#include <QFileDialog>

ActionView::ActionView(QWidget * parent, ActionView::WindowMode windowMode)
    : QDialog(parent)
    , ui(std::make_unique<Ui::ActionView>())
    , m_WindowMode(windowMode)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for (int i = 0; i < Action::ActionCount; ++i)
        ui->cbxType->addItem(Action::getTypeName((Action::ActionType)i));

    connectSlots();
}

ActionView::ActionView(QWidget * parent)
    : ActionView(parent, ActionCreate)
{
    m_Action = std::make_shared<Action>();
    ui->btnSave->setText("Create");
    onTypeSelChange(0);
}

ActionView::ActionView(QWidget * parent, std::shared_ptr<Action> action)
    : ActionView(parent, ActionEdit)
{
    Q_CHECK_PTR(action);
    m_Action = action;
    ui->btnSave->setText("Save");

    int typeIndex = static_cast<int>(action->getType());
    ui->tbxName->setText(action->getName());
    ui->cbxType->setCurrentIndex(typeIndex);
    ui->tbxSrcKey->setText(action->getSrcKeysName());

    onTypeSelChange(typeIndex);
}

ActionView::~ActionView() = default;

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
            ui->tbxAutoText->setVisible(false);
            ui->spButtons->changeSize(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
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
            ui->tbxAutoText->setVisible(false);
            ui->spButtons->changeSize(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
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
            ui->tbxAutoText->setVisible(false);
            ui->spButtons->changeSize(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
            break;
        }
        case Action::ActionAutoText:
        {
            ui->lblDstKey->setVisible(false);
            ui->tbxDstKey->setVisible(false);
            ui->btnDstKeyPlay->setVisible(false);
            ui->lblAppStart->setVisible(false);
            ui->tbxTargetPath->setVisible(false);
            ui->btnPicker->setVisible(false);
            ui->lblAppArgs->setVisible(false);
            ui->tbxAppArgs->setVisible(false);
            ui->tbxAutoText->setVisible(true);
            ui->spButtons->changeSize(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);
            break;
        }
    }
    layout()->invalidate();
}

ActionView::WindowMode ActionView::getWindowMode()
{
    return m_WindowMode;
}

void ActionView::onTypeSelChange(int index)
{
    auto type = static_cast<Action::ActionType>(index);
    updateVisibility(type);
    switch (type)
    {
        case Action::ActionKeyMap:
        default:
        {
            ui->tbxDstKey->setText(m_Action->getDstKeysName());
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
        case Action::ActionAutoText:
        {
            ui->tbxDstKey->clear();
            ui->tbxTargetPath->clear();
            ui->tbxAutoText->setPlainText(m_Action->getAutoText());
            break;
        }
    }
}

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
    auto    type    = static_cast<Action::ActionType>(ui->cbxType->currentIndex());

    QString filePath;
    if (type == Action::ActionAppLaunch)
        filePath =
            QFileDialog::getOpenFileName(this, "Get Target File", homeDir, "All files (*.*)");
    else if (type == Action::ActionDirLaunch)
        filePath = QFileDialog::getExistingDirectory(this, "Get Target Directory", homeDir);

    if (filePath.isEmpty()) return;

    ui->tbxTargetPath->setText(filePath);
}

void ActionView::onBtnCancel()
{
    close();
}

void ActionView::onBtnSave()
{
    if (ui->tbxName->text().isEmpty())
    {
        ui->tbxName->setFocus();
        return;
    }

    if (m_WindowMode == ActionCreate)
    {
        auto name = ui->tbxName->text();
        auto type = static_cast<Action::ActionType>(ui->cbxType->currentIndex());

        KeyboardKeys srcKeys, dstKeys;
        if (ui->tbxSrcKey->m_CurrentKeys) srcKeys = *ui->tbxSrcKey->m_CurrentKeys;
        if (ui->tbxDstKey->m_CurrentKeys) dstKeys = *ui->tbxDstKey->m_CurrentKeys;

        auto targetPath = ui->tbxTargetPath->text();
        auto appArgs    = ui->tbxAppArgs->text();

        Action action(name, type, srcKeys, dstKeys, targetPath, appArgs);
        action.setAutoText(ui->tbxAutoText->toPlainText());
        emit onCreated(action);
    }
    else
    {
        Action actionBackup(*m_Action);
        m_Action->reset();

        Action::ActionType type = static_cast<Action::ActionType>(ui->cbxType->currentIndex());

        m_Action->setName(ui->tbxName->text());
        m_Action->setType(type);

        m_Action->setSrcKeys(ui->tbxSrcKey->m_CurrentKeys ? *ui->tbxSrcKey->m_CurrentKeys
                                                          : actionBackup.getSrcKeys());

        m_Action->setDstKeys(ui->tbxDstKey->m_CurrentKeys ? *ui->tbxDstKey->m_CurrentKeys
                                                          : actionBackup.getDstKeys());

        m_Action->setTargetPath(ui->tbxTargetPath->text());
        m_Action->setAppArgs(ui->tbxAppArgs->text());
        m_Action->setAutoText(ui->tbxAutoText->toPlainText());

        emit onSaved();
    }

    close();
}
