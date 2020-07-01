
#pragma once

#include "QuickCutShared/Managers/ProfileManager.h"

#include <QAbstractTableModel>

class ActionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ActionsModel(QObject * parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex & index,
                 const QVariant &    value,
                 int                 role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex & index) const override;

private:
    ProfileManager m_Profiles;
};