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

    T * getById(const QString & uuid) const;
    T * getByName(const QString & name) const;
    T * getByIndex(int index) const;

    T *  create();
    T *  duplicate(int index);
    bool add(T * item);
    bool insert(int index, T * item);
    bool remove(int index);
    bool remove(T *& item);
    bool moveUp(int index);
    bool moveDown(int index);

    T *             operator[](int index);
    const T * const operator[](int index) const;

    typename std::vector<T *>::iterator       begin() { return m_Data.begin(); }
    typename std::vector<T *>::const_iterator begin() const { return m_Data.cbegin(); }
    typename std::vector<T *>::iterator       end() { return m_Data.end(); }
    typename std::vector<T *>::const_iterator end() const { return m_Data.cend(); }

protected:
    std::vector<T *> m_Data;
};
