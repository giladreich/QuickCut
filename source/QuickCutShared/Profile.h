
#pragma once

#include "Action.h"
#include <vector>

class Profile
{
public:
    Profile();
    Profile(std::string name);
    Profile(std::string id, std::string name, std::string lastModified);
    ~Profile();

    const std::string & getId() const;

    const std::string & getName() const;
    void                setName(const std::string & name);

    const std::string & getLastModified() const;

    void                          setActionsCapacity(int capacity);
    const std::vector<Action *> & getActions() const;
    int                           getActionsCount() const;

    bool addAction(Action * action);
    bool insertAction(uint32_t index, Action * action);
    bool deleteAction(Action * action);
    bool moveActionDown(int index);
    bool moveActionUp(int index);
    bool deleteActionByIndex(const uint32_t & index);

    void     getActionById(const std::string & uuid) const;
    void     getActionByName(const std::string & name) const;
    Action * getActionByIndex(const uint32_t & index) const;

    Action * operator[](uint32_t index);

private:
    void updated();

private:
    std::string m_Uuid;
    std::string m_Name;
    std::string m_LastModified;

    std::vector<Action *> m_Actions;
};
