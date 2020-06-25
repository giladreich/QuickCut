
#include "QuickCutShared/QuickCutPCH.h"
#include "Utility.h"

#include <QDateTime>
#include <QUuid>

#include <regex>

namespace boost
{
    namespace property_tree
    {
        void write_jsonEx(const std::string & path, const JSON & ptree)
        {
            std::ostringstream oss;
            bpt::write_json(oss, ptree);
            std::regex  reg("\\\"([0-9]+\\.{0,1}[0-9]*)\\\"");
            std::string result = std::regex_replace(oss.str(), reg, "$1");

            std::ofstream file;
            file.open(path);
            file << result;
            file.close();
        }

        QString get(JSON & ptree, const QString & path, const QString & defaultValue)
        {
            std::string result = ptree.get<std::string>(std::string(qPrintable(path)),
                                                        std::string(qPrintable(defaultValue)));
            return QString::fromStdString(result);
        }

        JSON & put(JSON & ptree, const QString & path, const QString & value)
        {
            return ptree.put(std::string(qPrintable(path)), std::string(qPrintable(value)));
        }
    } // namespace property_tree
} // namespace boost

namespace QuickCut
{
    QString getDateTime()
    {
        return QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
    }

    QString createUuid() { return QUuid::createUuid().toString(); }
} // namespace QuickCut