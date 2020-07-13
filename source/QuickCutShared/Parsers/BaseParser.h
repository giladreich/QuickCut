
#pragma once

#include "QuickCutShared/Utils/Utility.h"

#include <QFileInfo>

template <typename T>
class BaseParser
{
protected:
    BaseParser(const QString & path);
    BaseParser(QString && path);
    virtual ~BaseParser() = default;

    virtual bool saveImpl(const T & data) = 0;
    virtual bool parseImpl(T * outData)   = 0;

public:
    bool save(const T & data);
    bool parse(T * outData);

    const QString & getFilePath();

protected:
    JSON    m_Content;
    QString m_Path;
};

template <typename T>
inline BaseParser<T>::BaseParser(QString && path)
    : m_Path(std::move(path))
{
    m_Content.clear();
}

template <typename T>
inline BaseParser<T>::BaseParser(const QString & path)
    : m_Path(path)
{
    m_Content.clear();
}

template <typename T>
inline bool BaseParser<T>::save(const T & data)
{
    if (m_Path.isEmpty()) return false;

    QFileInfo fi(m_Path);
    if (!fi.dir().exists()) fi.dir().mkpath(".");

    m_Content.clear();

    if (!saveImpl(data)) return false;

    try
    {
        bpt::write_jsonEx(m_Path, m_Content);
    }
    catch (const bpt::ptree_error & e)
    {
        // TODO(Gilad): Consider using some logging system to produce log file.
        return false;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

template <typename T>
inline bool BaseParser<T>::parse(T * outData)
{
    if (m_Path.isEmpty() || !outData) return false;

    try
    {
        bpt::read_jsonEx(m_Path, m_Content);
    }
    catch (const bpt::ptree_error & e)
    {
        // TODO(Gilad): Consider using some logging system to produce log file.
        return false;
    }
    catch (...)
    {
        return false;
    }

    return parseImpl(outData);
}

template <typename T>
inline const QString & BaseParser<T>::getFilePath()
{
    return m_Path;
}
