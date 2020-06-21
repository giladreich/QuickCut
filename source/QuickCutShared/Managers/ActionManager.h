#pragma once

#include "BaseManager.h"
#include "Models/Action.h"

class ActionManager : public BaseManager<Action>
{
public:
    ActionManager() noexcept;
    ~ActionManager();

private:
};