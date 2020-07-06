
#include "QuickCutShared/QuickCutPCH.h"
#include "BaseManager.h"

#include "QuickCutShared/Models/Action.h"
#include "QuickCutShared/Models/Profile.h"

template class BaseManager<Action>;
template class BaseManager<Profile>;

template <typename T>
BaseManager<T>::BaseManager() noexcept
{
}

template <typename T>
BaseManager<T>::~BaseManager()
{
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
    m_Data.clear();
}

template <typename T>
void BaseManager<T>::setCapacity(int capacity)
{
    if (capacity < 0) return;

    m_Data.reserve(capacity);
}

template <typename T>
std::shared_ptr<T> BaseManager<T>::getById(const QString & uuid) const
{
    if (uuid.isEmpty()) return nullptr;

    auto itr = std::find_if(m_Data.begin(), m_Data.end(),
                            [&uuid](auto item) { return item->getId() == uuid; });
    if (itr == m_Data.end()) return nullptr;

    return *itr;
}

template <typename T>
std::shared_ptr<T> BaseManager<T>::getByName(const QString & name) const
{
    if (name.isEmpty()) return nullptr;

    auto itr = std::find_if(m_Data.begin(), m_Data.end(),
                            [&name](auto item) { return item->getName() == name; });
    if (itr == m_Data.end()) return nullptr;

    return *itr;
}

template <typename T>
std::shared_ptr<T> BaseManager<T>::getByIndex(int index) const
{
    if (index < 0 || index >= count()) return nullptr;

    return m_Data[index];
}

template <typename T>
std::shared_ptr<T> BaseManager<T>::create()
{
    std::shared_ptr<T> item = std::make_shared<T>();
    m_Data.emplace_back(item);
    return item;
}

template <typename T>
std::shared_ptr<T> BaseManager<T>::duplicate(int index)
{
    if (index < 0 || index >= count()) return nullptr;

    auto               itr  = std::next(m_Data.begin(), index);
    std::shared_ptr<T> copy = std::make_shared<T>(**itr);
    m_Data.emplace_back(copy);
    return copy;
}

template <typename T>
bool BaseManager<T>::add(std::shared_ptr<T> item)
{
    if (!item) return false;

    m_Data.emplace_back(item);
    return true;
}

template <typename T>
bool BaseManager<T>::insert(int index, std::shared_ptr<T> item)
{
    if (index < 0 || index >= count() || !item) return false;

    auto itr = std::next(m_Data.begin(), index + 1);
    m_Data.insert(itr, item);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(int index)
{
    if (index < 0 || index >= count()) return false;

    auto itr = std::next(m_Data.begin(), index);
    m_Data.erase(itr);
    return true;
}

template <typename T>
bool BaseManager<T>::remove(std::shared_ptr<T> & item)
{
    if (!item) return false;

    auto itr = std::find(m_Data.begin(), m_Data.end(), item);
    if (itr == m_Data.end()) return false;

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
std::shared_ptr<T> BaseManager<T>::operator[](int index)
{
    if (index < 0 || index >= count()) return nullptr;

    return m_Data[index];
}

template <typename T>
const std::shared_ptr<T> BaseManager<T>::operator[](int index) const
{
    if (index < 0 || index >= count()) return nullptr;

    return m_Data[index];
}
