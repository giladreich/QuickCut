
#include "QuickCutShared/QuickCutPCH.h"
#include "ProfileParser.h"

ProfileParser::ProfileParser(QString && path)
    : BaseParser(std::move(path))
{
}

ProfileParser::ProfileParser(const QString & path)
    : BaseParser(path)
{
}

bool ProfileParser::saveImpl(const std::vector<std::shared_ptr<Profile>> & data)
{
    QString activeProfileId = "";
    auto    profileItr =
        std::find_if(data.begin(), data.end(), [&](const std::shared_ptr<Profile> profile) {
            return profile->isActive();
        });
    if (profileItr != data.end()) activeProfileId = (*profileItr)->getId();

    bpt::put(m_Content, "activeProfile", activeProfileId);
    m_Content.put("profileCount", data.size());

    JSON profilesJson;
    for (auto && profile : data)
    {
        JSON profileJson;
        bpt::put(profileJson, "id", profile->getId());
        bpt::put(profileJson, "name", profile->getName());
        bpt::put(profileJson, "lastModified", profile->getLastModified());
        bpt::put(profileJson, "createdDate", profile->getCreatedDate());
        profileJson.put("actionsCount", profile->getActionManager().count());

        ActionManager & actions = profile->getActionManager();
        JSON            actionsJson;
        for (auto && action : actions)
        {
            JSON actionJson;
            auto actionType = action->getType();

            bpt::put(actionJson, "id", action->getId());
            bpt::put(actionJson, "actionName", action->getName());
            bpt::put(actionJson, "type", QuickCut::fromKey(actionType));
            if (actionType == Action::ActionFileLaunch ||
                actionType == Action::ActionDirLaunch)
                bpt::put(actionJson, "targetPath", action->getTargetPath());

            if (actionType == Action::ActionFileLaunch)
                bpt::put(actionJson, "targetArgs", action->getTargetArgs());

            if (actionType == Action::ActionAutoText)
                bpt::put(actionJson, "autoText", action->getAutoText());

            bpt::put(actionJson, "lastModified", action->getLastModified());
            bpt::put(actionJson, "createdDate", action->getCreatedDate());
            actionJson.put<bool>("enabled", action->isEnabled());
            actionJson.push_back(serializeKeys("srcKeys", action->getSrcKeys()));

            if (actionType == Action::ActionKeyMap)
                actionJson.push_back(serializeKeys("dstKeys", action->getDstKeys()));

            actionsJson.push_back(std::make_pair("", actionJson));
        }

        profileJson.push_back(std::make_pair("actions", actionsJson));
        profilesJson.push_back(std::make_pair("", profileJson));
    }
    m_Content.add_child("profiles", profilesJson);

    return true;
}

bool ProfileParser::parseImpl(std::vector<std::shared_ptr<Profile>> * outData)
{
    if (!outData->empty()) { outData->clear(); }

    QString activeProfileId = bpt::get(m_Content, "activeProfile", "");
    int     profileCount    = m_Content.get<int>("profileCount", 0);
    outData->reserve(profileCount);

    JSON profilesJson = m_Content.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        QString profileId    = bpt::get(profileJson.second, "id", "");
        QString profileName  = bpt::get(profileJson.second, "name", "");
        QString lastModified = bpt::get(profileJson.second, "lastModified", "");
        QString createdDate  = bpt::get(profileJson.second, "createdDate", "");
        int     actionsCount = profileJson.second.get<int>("actionsCount", 0);

        auto profile = std::make_shared<Profile>(profileId, lastModified, createdDate);
        profile->setName(profileName);
        profile->setActive(activeProfileId == profileId);
        profile->getActionManager().setCapacity(actionsCount);

        JSON actionsJson = profileJson.second.get_child("actions");
        for (auto && actionJson : actionsJson)
        {
            QString      actionId     = bpt::get(actionJson.second, "id", "");
            QString      actionName   = bpt::get(actionJson.second, "actionName", "");
            QString      actionType   = bpt::get(actionJson.second, "type", "");
            QString      targetPath   = bpt::get(actionJson.second, "targetPath", "");
            QString      targetArgs   = bpt::get(actionJson.second, "targetArgs", "");
            QString      autoText     = bpt::get(actionJson.second, "autoText", "");
            QString      lastModified = bpt::get(actionJson.second, "lastModified", "");
            QString      createdDate  = bpt::get(actionJson.second, "createdDate", "");
            bool         enabled      = actionJson.second.get<bool>("enabled", true);
            KeyboardKeys srcKeys      = deserializeKeys("srcKeys", actionJson.second);
            KeyboardKeys dstKeys      = deserializeKeys("dstKeys", actionJson.second);

            auto action = std::make_shared<Action>(actionId, lastModified, createdDate);
            action->setName(actionName);
            action->setType(QuickCut::fromValue<Action::ActionType>(actionType));
            action->setSrcKeys(srcKeys);
            action->setDstKeys(dstKeys);
            action->setTargetPath(targetPath);
            action->setTargetArgs(targetArgs);
            action->setAutoText(autoText);
            action->setEnabled(enabled);
            profile->getActionManager().add(action);
        }

        outData->emplace_back(profile);
    }

    return true;
}

std::pair<const char *, JSON> ProfileParser::serializeKeys(const char *         key,
                                                           const KeyboardKeys & keys)
{
    JSON keysJson;
    for (auto && k : keys)
    {
        JSON keyJson;
        bpt::put(keyJson, "keyName", k.getKeyName());
        bpt::put(keyJson, "keyCode", k.getKeyCodeHex());
        keysJson.push_back(std::make_pair("", keyJson));
    }

    return std::make_pair(key, keysJson);
}

KeyboardKeys ProfileParser::deserializeKeys(const char * key, JSON actionJson)
{
    auto keysJson = actionJson.get_child_optional(key);
    if (!keysJson) return {};

    KeyboardKeys keys;
    keys.reserve(keysJson->size());
    for (auto && k : *keysJson)
    {
        QString keyName = bpt::get(k.second, "keyName", "");
        QString keyCode = bpt::get(k.second, "keyCode", "");
        keys.push_back(KeyData(keyName, keyCode));
    }

    return keys;
}
