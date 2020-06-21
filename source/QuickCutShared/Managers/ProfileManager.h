
#pragma once

#include "BaseManager.h"
#include "IParserOperations.h"
#include "Parser/ProfileParser.h"

class ProfileManager : public BaseManager<Profile>, IParserOperations
{
public:
    ProfileManager() noexcept;
    ProfileManager(const std::string & filePath) noexcept;
    ProfileManager(std::string && filePath) noexcept;
    ~ProfileManager();

    bool load() override;
    bool save() override;

    const std::string & getConfigFilePath() override;

    Profile * getActiveProfile();
    void      setActiveProfile(int index);
    void      setActiveProfile(Profile * profile);

private:
    ProfileParser m_Parser;
};
