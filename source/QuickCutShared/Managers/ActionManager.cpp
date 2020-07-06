
#include "QuickCutShared/QuickCutPCH.h"
#include "ActionManager.h"

ActionManager::ActionManager() noexcept
    : BaseManager()
{
}

ActionManager::ActionManager(const ActionManager & other)
{
    m_Data.reserve(other.count());
    for (auto && action : other) m_Data.emplace_back(std::make_shared<Action>(*action));
}

ActionManager::~ActionManager() = default;

ActionManager & ActionManager::operator=(const ActionManager & other)
{
    if (this != &other)
    {
        std::vector<std::shared_ptr<Action>> tmpActions;
        tmpActions.reserve(other.count());
        for (auto && action : other)
            tmpActions.emplace_back(std::make_shared<Action>(*action));

        clear();
        m_Data = tmpActions;
    }

    return *this;
}
