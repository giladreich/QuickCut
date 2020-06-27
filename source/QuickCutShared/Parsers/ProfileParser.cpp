
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

ProfileParser::~ProfileParser() {}

bool ProfileParser::parseImpl(std::vector<Profile *> * outData)
{
    if (!outData->empty())
    {
        for (auto && profile : *outData) delete profile;
        outData->clear();
    }

    QString activeProfileId = bpt::get(m_Content, "activeProfile", "");
    int     profileCount    = m_Content.get<int>("profileCount", 0);
    outData->reserve(profileCount);

    JSON profilesJson = m_Content.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        QString profileId    = bpt::get(profileJson.second, "id", "");
        QString profileName  = bpt::get(profileJson.second, "name", "");
        QString lastModified = bpt::get(profileJson.second, "lastModified", "");
        int     actionsCount = profileJson.second.get<int>("actionsCount", 0);

        Profile * profile = new Profile(std::move(profileId), std::move(profileName),
                                        std::move(lastModified), activeProfileId == profileId);
        profile->getActionManager().setCapacity(actionsCount);

        JSON actionsJson = profileJson.second.get_child("actions");
        for (auto && actionJson : actionsJson)
        {
            QString actionId     = bpt::get(actionJson.second, "id", "");
            QString actionName   = bpt::get(actionJson.second, "actionName", "");
            QString actionType   = bpt::get(actionJson.second, "type", "");
            QString srcKey       = bpt::get(actionJson.second, "srcKeys", "");
            QString dstKey       = bpt::get(actionJson.second, "dstKeys", "");
            QString targetPath   = bpt::get(actionJson.second, "targetPath", "");
            QString appArgs      = bpt::get(actionJson.second, "appArgs", "");
            QString createdDate  = bpt::get(actionJson.second, "createdDate", "");
            QString lastModified = bpt::get(actionJson.second, "lastModified", "");

            profile->getActionManager().add(
                new Action(actionId, actionName, lastModified,
                           QuickCut::fromValue<Action::ActionType>(actionType), srcKey, dstKey,
                           targetPath, appArgs, createdDate));
        }

        outData->emplace_back(profile);
    }

    return true;
}

bool ProfileParser::saveImpl(const std::vector<Profile *> & data)
{
    QString activeProfileId = "";
    auto    profileItr = std::find_if(data.begin(), data.end(), [&](const Profile * profile) {
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
        profileJson.put("actionsCount", profile->getActionManager().count());

        JSON actionsJson;
        for (auto && action : profile->getActionManager())
        {
            JSON actionJson;
            bpt::put(actionJson, "id", action->getId());
            bpt::put(actionJson, "actionName", action->getName());
            bpt::put(actionJson, "type", QuickCut::fromKey(action->getType()));
            bpt::put(actionJson, "srcKeys", action->getSrcKeys());
            bpt::put(actionJson, "dstKeys", action->getDstKeys());
            bpt::put(actionJson, "targetPath", action->getTargetPath());
            bpt::put(actionJson, "appArgs", action->getAppArgs());
            bpt::put(actionJson, "createdDate", action->getCreatedDate());
            bpt::put(actionJson, "lastModified", action->getLastModified());

            actionsJson.push_back(std::make_pair("", actionJson));
        }

        profileJson.push_back(std::make_pair("actions", actionsJson));
        profilesJson.push_back(std::make_pair("", profileJson));
    }
    m_Content.add_child("profiles", profilesJson);

    return true;
}
