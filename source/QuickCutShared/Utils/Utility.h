
#pragma once

#include "QuickCutShared/Types.h"

namespace boost
{
    namespace property_tree
    {
        /*
         * Originally the boost::write_json function treats even numbers as strings, therefore
         * this function will parse the input and make the json file look as it should be.
         */
        void write_jsonEx(const std::string & path, const JSON & ptree);

        QString get(JSON & ptree, const QString & path, const QString & defaultValue);
        JSON &  put(JSON & ptree, const QString & path, const QString & value);
    } // namespace property_tree
} // namespace boost

namespace QuickCut
{
    QString getDateTime();
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
} // namespace QuickCut
