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

    T * getById(const std::string & uuid) const;
    T * getByName(const std::string & name) const;
    T * getByIndex(int index) const;

    T *  create();
    T *  duplicate(int index);
    bool add(T * item);
    bool insert(int index, T * item);
    bool remove(int index);
    bool remove(T *& item);
    bool moveUp(int index);
    bool moveDown(int index);

    T *                                       operator[](int index);
    typename std::vector<T *>::iterator       begin() { return m_vData.begin(); }
    typename std::vector<T *>::const_iterator begin() const { return m_vData.cbegin(); }
    typename std::vector<T *>::iterator       end() { return m_vData.end(); }
    typename std::vector<T *>::const_iterator end() const { return m_vData.cend(); }

protected:
    std::vector<T *> m_vData;
};
