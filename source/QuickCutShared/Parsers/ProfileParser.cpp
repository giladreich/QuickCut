
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
            bpt::put(actionJson, "id", action->getId());
            bpt::put(actionJson, "actionName", action->getName());
            bpt::put(actionJson, "type", QuickCut::fromKey(action->getType()));
            bpt::put(actionJson, "targetPath", action->getTargetPath());
            bpt::put(actionJson, "targetArgs", action->getTargetArgs());
            if (action->getType() == Action::ActionAutoText)
                bpt::put(actionJson, "autoText", action->getAutoText());
            bpt::put(actionJson, "lastModified", action->getLastModified());
            bpt::put(actionJson, "createdDate", action->getCreatedDate());
            actionJson.put<bool>("enabled", action->isEnabled());

            JSON srcKeysJson;
            for (auto && srcKey : action->getSrcKeys())
            {
                JSON keyJson;
                bpt::put(keyJson, "keyName", srcKey.getKeyName());
                bpt::put(keyJson, "keyCode", srcKey.getKeyCodeHex());
                srcKeysJson.push_back(std::make_pair("", keyJson));
            }
            actionJson.push_back(std::make_pair("srcKeys", srcKeysJson));

            JSON dstKeysJson;
            for (auto && dstKey : action->getDstKeys())
            {
                JSON keyJson;
                bpt::put(keyJson, "keyName", dstKey.getKeyName());
                bpt::put(keyJson, "keyCode", dstKey.getKeyCodeHex());
                dstKeysJson.push_back(std::make_pair("", keyJson));
            }
            actionJson.push_back(std::make_pair("dstKeys", dstKeysJson));

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
            QString actionId   = bpt::get(actionJson.second, "id", "");
            QString actionName = bpt::get(actionJson.second, "actionName", "");
            QString actionType = bpt::get(actionJson.second, "type", "");
            QString targetPath = bpt::get(actionJson.second, "targetPath", "");
            QString targetArgs = bpt::get(actionJson.second, "targetArgs", "");
            QString autoText   = "";
            if (QuickCut::fromValue<Action::ActionType>(actionType) == Action::ActionAutoText)
                autoText = bpt::get(actionJson.second, "autoText", "");
            QString lastModified = bpt::get(actionJson.second, "lastModified", "");
            QString createdDate  = bpt::get(actionJson.second, "createdDate", "");
            bool    enabled      = actionJson.second.get<bool>("enabled", true);

            KeyboardKeys srcKeys;
            JSON         srcKeysJson = actionJson.second.get_child("srcKeys");
            srcKeys.reserve(srcKeysJson.size());
            for (auto && srcKey : srcKeysJson)
            {
                QString keyName = bpt::get(srcKey.second, "keyName", "");
                QString keyCode = bpt::get(srcKey.second, "keyCode", "");
                srcKeys.push_back(KeyData(keyName, keyCode));
            }

            KeyboardKeys dstKeys;
            JSON         dstKeysJson = actionJson.second.get_child("dstKeys");
            dstKeys.reserve(dstKeysJson.size());
            for (auto && dstKey : dstKeysJson)
            {
                QString keyName = bpt::get(dstKey.second, "keyName", "");
                QString keyCode = bpt::get(dstKey.second, "keyCode", "");
                dstKeys.push_back(KeyData(keyName, keyCode));
            }

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
