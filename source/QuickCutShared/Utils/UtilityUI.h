
#pragma once

#include <QLineEdit>
#include <QTextLayout>
#include <QTableWidget>

namespace QuickCut
{
    void setLineEditTextFormat(class QLineEdit *                       tbx,
                               const QList<QTextLayout::FormatRange> & formats);

    void moveItemUp(QTableWidget * table, bool moveUp = true);
} // namespace QuickCut
