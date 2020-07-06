#pragma once

#include <vector>
#include <string>

template <typename T>
class BaseManager
{
protected:
    BaseManager() noexcept;
    virtual ~BaseManager();

public:
    int  count() const;
    bool empty() const;
    void clear();
    void setCapacity(int capacity);

    std::shared_ptr<T> getById(const QString & uuid) const;
    std::shared_ptr<T> getByName(const QString & name) const;
    std::shared_ptr<T> getByIndex(int index) const;

    std::shared_ptr<T> create();
    std::shared_ptr<T> duplicate(int index);
    bool               add(std::shared_ptr<T> item);
    bool               insert(int index, std::shared_ptr<T> item);
    bool               remove(int index);
    bool               remove(std::shared_ptr<T> & item);
    bool               moveUp(int index);
    bool               moveDown(int index);

    std::shared_ptr<T>       operator[](int index);
    const std::shared_ptr<T> operator[](int index) const;

    typename std::vector<std::shared_ptr<T>>::iterator       begin() { return m_Data.begin(); }
    typename std::vector<std::shared_ptr<T>>::const_iterator begin() const
    {
        return m_Data.cbegin();
    }
    typename std::vector<std::shared_ptr<T>>::iterator       end() { return m_Data.end(); }
    typename std::vector<std::shared_ptr<T>>::const_iterator end() const
    {
        return m_Data.cend();
    }

protected:
    std::vector<std::shared_ptr<T>> m_Data;
};
