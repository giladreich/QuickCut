#pragma once

#include "BaseManager.h"
#include "QuickCutShared/Models/Action.h"

class ActionManager : public BaseManager<Action>
{
public:
    ActionManager() noexcept;

    ActionManager(const ActionManager & other);
    ActionManager(ActionManager && other) = default;

    ~ActionManager();

    ActionManager & operator=(const ActionManager & other);
    ActionManager & operator=(ActionManager && other) = default;
};