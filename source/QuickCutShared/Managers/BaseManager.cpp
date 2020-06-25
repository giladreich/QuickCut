
#include "QuickCutPCH.h"
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
    for (auto && item : m_Data) delete item;
    m_Data.clear();
}

template <typename T>
int BaseManager<T>::count() const
{
    return (int)m_Data.size();
}

template <typename T>
bool BaseManager<T>::empty() const
{
    return count() == 0;
}

template <typename T>
void BaseManager<T>::clear()
{
    for (auto && item : m_Data) delete item;
    m_Data.clear();
}

template <typename T>
void BaseManager<T>::setCapacity(int capacity)
{
    if (capacity < 0) return;

    m_Data.reserve(capacity);
}

template <typename T>
T * BaseManager<T>::getById(const QString & uuid) const
{
    if (uuid.isEmpty()) return nullptr;

    auto itr = std::find_if(m_Data.begin(), m_Data.end(),
                            [&uuid](auto item) { return item->getId() == uuid; });
    if (itr == m_Data.end()) return nullptr;

    return *itr;
}

template <typename T>
T * BaseManager<T>::getByName(const QString & name) const
{
    if (name.isEmpty()) return nullptr;

    auto itr = std::find_if(m_Data.begin(), m_Data.end(),
                            [&name](auto item) { return item->getName() == name; });
    if (itr == m_Data.end()) return nullptr;

    return *itr;
}

template <typename T>
T * BaseManager<T>::getByIndex(int index) const
{
    if (index < 0 || index >= count()) return nullptr;

    return m_Data[index];
}

template <typename T>
T * BaseManager<T>::create()
{
    T * item = new T();
    m_Data.emplace_back(item);
    return item;
}

template <typename T>
T * BaseManager<T>::duplicate(int index)
{
    if (index < 0 || index >= count()) return nullptr;

    auto itr = m_Data.begin();
    std::advance(itr, index);
    T * copy = new T(**itr);
    m_Data.emplace_back(copy);
    return copy;
}

template <typename T>
bool BaseManager<T>::add(T * item)
{
    if (!item) return false;

    m_Data.emplace_back(item);
    return true;
}

template <typename T>
bool BaseManager<T>::insert(int index, T * item)
{
    if (index < 0 || index >= count() || !item) return false;

    auto itr = m_Data.begin();
    std::advance(itr, index + 1);
    m_Data.insert(itr, item);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(int index)
{
    if (index < 0 || index >= count()) return false;

    auto itr = m_Data.begin();
    std::advance(itr, index);
    delete *itr;
    m_Data.erase(itr);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(T *& item)
{
    if (!item) return false;

    auto itr = std::find(m_Data.begin(), m_Data.end(), item);
    if (itr == m_Data.end()) return false;

    delete item;
    item = nullptr;
    m_Data.erase(itr);
    return true;
}

template <typename T>
bool BaseManager<T>::moveUp(int index)
{
    if (m_Data.empty() || index <= 0 || index >= count()) return false;

    std::swap(m_Data[index], m_Data[index - 1]);
    return true;
}

template <typename T>
bool BaseManager<T>::moveDown(int index)
{
    if (m_Data.empty() || index < 0 || index >= count() - 1) return false;

    std::swap(m_Data[index], m_Data[index + 1]);
    return true;
}

template <typename T>
T * BaseManager<T>::operator[](int index)
{
    if (index < 0 || index >= count()) return nullptr;

    return m_Data[index];
}
