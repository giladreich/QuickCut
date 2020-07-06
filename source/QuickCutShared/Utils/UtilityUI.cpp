
#include "QuickCutShared/QuickCutPCH.h"
#include "UtilityUI.h"
#include <QInputMethodEvent>
#include <QCoreApplication>

namespace QuickCut
{
    void setLineEditTextFormat(QLineEdit *                             tbx,
                               const QList<QTextLayout::FormatRange> & formats)
    {
        if (!tbx || formats.isEmpty()) return;

        QList<QInputMethodEvent::Attribute> attributes;
        for (auto && format : formats)
        {
            attributes.append(QInputMethodEvent::Attribute(
                QInputMethodEvent::TextFormat, format.start - tbx->cursorPosition(),
                format.length, format.format));
        }
        QInputMethodEvent event(QString(), attributes);
        QCoreApplication::sendEvent(tbx, &event);
    }

    void moveItemUp(QTableWidget * table, bool moveUp /* = true*/)
    {
        const int currIndex = table->currentRow();
        if (currIndex == -1) return;

        if (moveUp)
        {
            if (currIndex < 1) return;

            for (int col = 0; col < table->columnCount(); ++col)
            {
                QTableWidgetItem * itemAbove = table->takeItem(currIndex - 1, col);
                QTableWidgetItem * item      = table->takeItem(currIndex, col);
                table->setItem(currIndex - 1, col, item);
                table->setItem(currIndex, col, itemAbove);
            }
            table->selectRow(currIndex - 1);
        }
        else
        {
            if (currIndex >= table->rowCount() - 1) return;

            for (int col = 0; col < table->columnCount(); ++col)
            {
                QTableWidgetItem * itemBelow = table->takeItem(currIndex + 1, col);
                QTableWidgetItem * item      = table->takeItem(currIndex, col);
                table->setItem(currIndex + 1, col, item);
                table->setItem(currIndex, col, itemBelow);
            }
            table->selectRow(currIndex + 1);
        }
    }
} // namespace QuickCut