
#include "pch.h"
#include "ProfileParser.h"

ProfileParser::ProfileParser(std::string && path)
    : BaseParser(std::move(path))
{
}

ProfileParser::ProfileParser(const std::string & path)
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

    std::string activeProfileId = m_Content.get<std::string>("activeProfile", "");
    int         profileCount      = m_Content.get<int>("profileCount", 0);
    outData->reserve(profileCount);

    JSON profilesJson = m_Content.get_child("profiles");
    for (auto && profileJson : profilesJson)
    {
        std::string profileId    = profileJson.second.get<std::string>("id", "");
        std::string profileName  = profileJson.second.get<std::string>("name", "");
        std::string lastModified = profileJson.second.get<std::string>("lastModified", "");
        int         actionsCount = profileJson.second.get<int>("actionsCount", 0);

        Profile * profile =
            new Profile(std::move(profileId), std::move(profileName), std::move(lastModified),
                        activeProfileId == profileId);
        profile->setActionsCapacity(actionsCount);

        JSON actionsJson = profileJson.second.get_child("actions");
        for (auto && actionJson : actionsJson)
        {
            std::string actionId     = actionJson.second.get<std::string>("id", "");
            std::string actionName   = actionJson.second.get<std::string>("actionName", "");
            std::string actionType   = actionJson.second.get<std::string>("type", "");
            std::string srcKey       = actionJson.second.get<std::string>("srcKey", "");
            std::string dstKey       = actionJson.second.get<std::string>("dstKey", "");
            std::string appPath      = actionJson.second.get<std::string>("appPath", "");
            std::string appArgs      = actionJson.second.get<std::string>("appArgs", "");
            std::string createdDate  = actionJson.second.get<std::string>("createdDate", "");
            std::string lastModified = actionJson.second.get<std::string>("lastModified", "");

            profile->addAction(new Action(actionId, actionName, lastModified,
                                           Action::getType(actionType), srcKey, dstKey,
                                           appPath, appArgs, createdDate));
        }

        outData->emplace_back(profile);
    }

    return true;
}

bool ProfileParser::saveImpl(const std::vector<Profile *> & data)
{
    std::string activeProfileId = "";
    auto profileItr = std::find_if(data.begin(), data.end(), [&](const Profile * profile) {
        return profile->isActive();
    });
    if (profileItr != data.end()) activeProfileId = (*profileItr)->getId();

    m_Content.put("activeProfile", activeProfileId);
    m_Content.put("profileCount", data.size());

    JSON profilesJson;
    for (auto && profile : data)
    {
        JSON profileJson;
        profileJson.put("id", profile->getId());
        profileJson.put("name", profile->getName());
        profileJson.put("lastModified", profile->getLastModified());
        profileJson.put("actionsCount", profile->getActionsCount());

        JSON actionsJson;
        for (auto && action : profile->getActions())
        {
            JSON actionJson;
            actionJson.put("id", action->getId());
            actionJson.put("actionName", action->getName());
            actionJson.put("type", Action::getType(action->getType()));
            actionJson.put("srcKey", action->getSrcKey());
            actionJson.put("dstKey", action->getDstKey());
            actionJson.put("appPath", action->getAppPath());
            actionJson.put("appArgs", action->getAppArgs());
            actionJson.put("createdDate", action->getCreatedDate());
            actionJson.put("lastModified", action->getLastModified());

            actionsJson.push_back(std::make_pair("", actionJson));
        }

        profileJson.push_back(std::make_pair("actions", actionsJson));
        profilesJson.push_back(std::make_pair("", profileJson));
    }
    m_Content.add_child("profiles", profilesJson);

    return true;
}
