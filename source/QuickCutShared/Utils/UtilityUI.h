
#pragma once

#include <QLineEdit>
#include <QTextLayout>

namespace QuickCut
{
    void setLineEditTextFormat(class QLineEdit *                       tbx,
                               const QList<QTextLayout::FormatRange> & formats);
} // namespace QuickCut
