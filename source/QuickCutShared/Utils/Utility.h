
#pragma once

#include "QuickCutShared/Types.h"

namespace boost
{
    namespace property_tree
    {

#if defined(UNICODE)
        void read_jsonEx(const QString & path, bpt::wptree & pt);
#else
        void read_jsonEx(const QString & path, bpt::ptree & pt);
#endif

#if defined(UNICODE)
        void write_jsonEx(const QString & path, const bpt::wptree & pt);
#else
        void write_jsonEx(const QString & path, const bpt::ptree & pt);
#endif

#if defined(UNICODE)
        template <typename Type>
        inline Type get(bpt::wptree & pt, const QString & path, const Type & defaultValue)
        {
            return pt.get<Type>(std::wstring(qUtf16Printable(path)), defaultValue);
        }

        template <>
        inline QString
            get(bpt::wptree & pt, const QString & path, const QString & defaultValue)
        {
            auto result = pt.get<std::wstring>(std::wstring(qUtf16Printable(path)),
                                               std::wstring(qUtf16Printable(defaultValue)));
            return QString::fromStdWString(result);
        }
#else
        template <typename Type>
        inline Type get(bpt::ptree & pt, const QString & path, const Type & defaultValue)
        {
            return pt.get<Type>(std::string(qPrintable(path)), defaultValue);
        }

        template <>
        inline QString get(bpt::ptree & pt, const QString & path, const QString & defaultValue)
        {
            auto result = pt.get<std::string>(std::string(qPrintable(path)),
                                              std::string(qPrintable(defaultValue)));
            return QString::fromStdString(result);
        }
#endif

#if defined(UNICODE)
        template <typename Type>
        inline bpt::wptree & put(bpt::wptree & pt, const QString & path, const Type & value)
        {
            return pt.put<Type>(std::wstring(qUtf16Printable(path)), value);
        }

        template <>
        inline bpt::wptree & put(bpt::wptree & pt, const QString & path, const QString & value)
        {
            auto v = std::wstring(qUtf16Printable(value));
            return pt.put<std::wstring>(std::wstring(qUtf16Printable(path)), v);
        }
#else
        template <typename Type>
        inline bpt::ptree & put(bpt::ptree & pt, const QString & path, const Type & value)
        {
            return pt.put<Type>(std::string(qPrintable(path)), value);
        }

        template <>
        inline bpt::ptree & put(bpt::ptree & pt, const QString & path, const QString & value)
        {
            auto v = std::string(qPrintable(value));
            return pt.put<std::string>(std::string(qPrintable(path)), v);
        }
#endif

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
