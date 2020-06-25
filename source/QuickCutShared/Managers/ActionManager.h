#pragma once

#include "BaseManager.h"
#include "QuickCutShared/Models/Action.h"

class ActionManager : public BaseManager<Action>
{
public:
    ActionManager() noexcept;
    ~ActionManager();

private:
};