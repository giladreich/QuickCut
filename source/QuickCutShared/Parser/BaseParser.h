
#pragma once

#include "Utils/Utility.h"

template <typename T>
class BaseParser
{
public:
    BaseParser(const std::string & path);
    BaseParser(std::string && path);
    virtual ~BaseParser();

    bool parse(T * outData);
    bool save(const T & data);

    const std::string & getFilePath();

protected:
    virtual bool parseImpl(T * outData)   = 0;
    virtual bool saveImpl(const T & data) = 0;

protected:
    JSON        m_Content;
    std::string m_Path;
};

template <typename T>
inline BaseParser<T>::BaseParser(std::string && path)
    : m_Path(std::move(path))
{
    m_Content.clear();
}

template <typename T>
inline BaseParser<T>::BaseParser(const std::string & path)
    : m_Path(path)
{
    m_Content.clear();
}

template <typename T>
inline BaseParser<T>::~BaseParser()
{
}

template <typename T>
inline bool BaseParser<T>::parse(T * outData)
{
    if (m_Path.empty() || !outData) return false;

    bpt::read_json(m_Path, m_Content);

    return parseImpl(outData);
}

template <typename T>
inline bool BaseParser<T>::save(const T & data)
{
    if (m_Path.empty()) return false;

    m_Content.clear();

    if (!saveImpl(data)) return false;

    bpt::write_jsonEx(m_Path, m_Content);

    return true;
}

template <typename T>
inline const std::string & BaseParser<T>::getFilePath()
{
    return m_Path;
}
