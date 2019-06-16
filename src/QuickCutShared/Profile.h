
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
    void setName(const std::string & name);

    const std::string & getLastModified() const;

    void setActionsCapacity(int capacity);
    const std::vector<Action *> & getActions() const;
    int getActionsCount() const;

    bool addAction(Action * action);
    bool insertAction(uint32_t iIndex, Action * pAction);
    bool deleteAction(Action * action);
    bool moveActionDown(int iIndex);
    bool moveActionUp(int iIndex);
    bool deleteActionByIndex(const uint32_t & iIndex);

    void getActionById(const std::string & uuid) const;
    void getActionByName(const std::string & name) const;
    Action * getActionByIndex(const uint32_t & iIndex) const;

    Action * operator [] (uint32_t iIndex);


private:
    void updated();


private:
    std::string m_szUuid;
    std::string m_szName;
    std::string m_szLastModified;

    std::vector<Action *> m_vActions;

};




