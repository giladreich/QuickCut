
#pragma once

#include "QuickCutShared/Types.h"

namespace boost
{
    namespace property_tree
    {
        void write_jsonEx(const QString & path, const JSON & ptree);
        void read_jsonEx(const QString & path, JSON & ptree);

        QString get(JSON & ptree, const QString & path, const QString & defaultValue);
        JSON &  put(JSON & ptree, const QString & path, const QString & value);
    } // namespace property_tree
} // namespace boost

namespace QuickCut
{
    QString getDateTime();
    QString getCurrentOSName();
    QString createUuid();

    template <typename QEnum>
    QString fromKey(const QEnum key)
    {
        return QString(QMetaEnum::fromType<QEnum>().valueToKey(key));
    }

    // Returns -1 if could not find.
    template <typename QEnum>
    QEnum fromValue(const QString & value)
    {
        return static_cast<QEnum>(QMetaEnum::fromType<QEnum>().keyToValue(qPrintable(value)));
    }

    void logToFile(const QString & filePath, const QString & text);

} // namespace QuickCut
