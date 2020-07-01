
#include "QuickCutShared/QuickCutPCH.h"
#include "ActionsModel.h"

ActionsModel::ActionsModel(QObject * parent)
    : QAbstractTableModel(parent)
{
}

QVariant ActionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return QString("Keys");
            case 1:
                return QString("Description");
        }
    }

    return QVariant();
}

int ActionsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_Profiles.count();
}

int ActionsModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant ActionsModel::data(const QModelIndex & index, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
            int  row     = index.row();
            int  col     = index.column();
            auto profile = m_Profiles.getActiveProfile();
            auto action  = profile->getActionManager()[row];

            if (col == 0)
            {
                if (action->getType() == Action::ActionKeyMap)
                    return QString("[%1] ~ [%2]")
                        .arg(action->getSrcKeysName())
                        .arg(action->getDstKeysName());
                else
                    return QString("[%1]").arg(action->getSrcKeysName());
            }
            else if (col == 1)
            {
                return QString(action->getName());
            }

            return QString();
    }

    return QVariant();
}

bool ActionsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        if (!checkIndex(index)) return false;

        int row = index.row();
        int col = index.column();

        auto profile = m_Profiles.getActiveProfile();
        auto action  = profile->getActionManager()[row];

        return true;
    }
    return false;
}

Qt::ItemFlags ActionsModel::flags(const QModelIndex & index) const
{
    return Qt::ItemIsSelectable | QAbstractTableModel::flags(index);
}
