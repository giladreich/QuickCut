
#include "pch.h"
#include "BaseManager.h"

#include "Models/Action.h"
#include "Models/Profile.h"

template class BaseManager<Action>;
template class BaseManager<Profile>;

template <typename T>
BaseManager<T>::BaseManager() noexcept
{
}

template <typename T>
BaseManager<T>::~BaseManager()
{
    for (auto && item : m_vData) delete item;
    m_vData.clear();
}

template <typename T>
int BaseManager<T>::count() const
{
    return (int)m_vData.size();
}

template <typename T>
bool BaseManager<T>::empty() const
{
    return count() == 0;
}

template <typename T>
void BaseManager<T>::clear()
{
    for (auto && item : m_vData) delete item;
    m_vData.clear();
}

template <typename T>
void BaseManager<T>::setCapacity(int capacity)
{
    if (capacity < 0) return;

    m_vData.reserve(capacity);
}

template <typename T>
T * BaseManager<T>::getById(const std::string & uuid) const
{
    if (uuid.empty()) return nullptr;

    auto itr = std::find_if(m_vData.begin(), m_vData.end(),
                            [&uuid](auto item) { return item->getId() == uuid; });
    if (itr == m_vData.end()) return nullptr;

    return *itr;
}

template <typename T>
T * BaseManager<T>::getByName(const std::string & name) const
{
    if (name.empty()) return nullptr;

    auto itr = std::find_if(m_vData.begin(), m_vData.end(),
                            [&name](auto item) { return item->getName() == name; });
    if (itr == m_vData.end()) return nullptr;

    return *itr;
}

template <typename T>
T * BaseManager<T>::getByIndex(int index) const
{
    if (index < 0 || index >= count()) return nullptr;

    return m_vData[index];
}

template <typename T>
T * BaseManager<T>::create()
{
    T * item = new T();
    m_vData.emplace_back(item);
    return item;
}

template <typename T>
T * BaseManager<T>::duplicate(int index)
{
    if (index < 0 || index >= count()) return nullptr;

    auto itr = m_vData.begin();
    std::advance(itr, index);
    T * copy = new T(**itr);
    m_vData.emplace_back(copy);
    return copy;
}

template <typename T>
bool BaseManager<T>::add(T * item)
{
    if (!item) return false;

    m_vData.emplace_back(item);
    return true;
}

template <typename T>
bool BaseManager<T>::insert(int index, T * item)
{
    if (index < 0 || index >= count() || !item) return false;

    auto itr = m_vData.begin();
    std::advance(itr, index + 1);
    m_vData.insert(itr, item);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(int index)
{
    if (index < 0 || index >= count()) return false;

    auto itr = m_vData.begin();
    std::advance(itr, index);
    delete *itr;
    m_vData.erase(itr);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(T *& item)
{
    if (!item) return false;

    auto itr = std::find(m_vData.begin(), m_vData.end(), item);
    if (itr == m_vData.end()) return false;

    delete item;
    item = nullptr;
    m_vData.erase(itr);
    return true;
}

template <typename T>
bool BaseManager<T>::moveUp(int index)
{
    if (m_vData.empty() || index <= 0 || index >= count()) return false;

    std::swap(m_vData[index], m_vData[index - 1]);
    return true;
}

template <typename T>
bool BaseManager<T>::moveDown(int index)
{
    if (m_vData.empty() || index < 0 || index >= count() - 1) return false;

    std::swap(m_vData[index], m_vData[index + 1]);
    return true;
}

template <typename T>
T * BaseManager<T>::operator[](int index)
{
    if (index < 0 || index >= count()) return nullptr;

    return m_vData[index];
}
