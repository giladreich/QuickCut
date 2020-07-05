
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
} // namespace QuickCut