
#include "QuickCutShared/QuickCutPCH.h"
#include "Utility.h"

#include <QDateTime>
#include <QUuid>

#include <regex>

namespace boost
{
    namespace property_tree
    {
#if defined(UNICODE)
        void read_jsonEx(const QString & path, bpt::wptree & ptree)
        {
            bpt::read_json(qPrintable(path), ptree);
        }
#else
        void read_jsonEx(const QString & path, bpt::ptree & ptree)
        {
            bpt::read_json(qPrintable(path), ptree);
        }
#endif

#if defined(UNICODE)
        void write_jsonEx(const QString & path, const bpt::wptree & ptree)
        {
            std::wostringstream oss;
            bpt::write_json(oss, ptree);

            static std::wregex reg(L"\\\"([0-9]+\\.{0,1}[0-9]*)\\\"");
            std::wstring       output = std::regex_replace(oss.str(), reg, L"$1");

            std::wofstream file;
            file.open(qPrintable(path));
            file << output;
            file.close();
        }
#else
        void write_jsonEx(const QString & path, const bpt::ptree & ptree)
        {
            std::ostringstream oss;
            bpt::write_json(oss, ptree);

            static std::regex reg("\\\"([0-9]+\\.{0,1}[0-9]*)\\\"");
            std::string       output = std::regex_replace(oss.str(), reg, "$1");

            std::ofstream file;
            file.open(qPrintable(path));
            file << output;
            file.close();
        }
#endif
    } // namespace property_tree
} // namespace boost

namespace QuickCut
{
    QString getDateTime()
    {
        return QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
    }

    QString getCurrentOSName()
    {
#if defined(Q_OS_MACOS)
        return QLatin1String("MacOS");
#elif defined(Q_OS_WIN)
        return QLatin1String("Windows");
#elif defined(Q_OS_LINUX)
        return QLatin1String("Linux");
#elif defined(Q_OS_UNIX)
        return QLatin1String("Unix");
#else
        return QLatin1String("Unknown");
#endif
    }

    QString createUuid() { return QUuid::createUuid().toString(); }

    void logToFile(const QString & filePath, const QString & text)
    {
        QFile file(filePath);
        file.open(QFile::WriteOnly | QFile::Append);
        QTextStream ts(&file);
        ts << text << QLatin1Char('\n');
        file.flush();
        file.close();
    }
} // namespace QuickCut
