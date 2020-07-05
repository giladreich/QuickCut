
#pragma once

#include "BaseManager.h"
#include "IParserOperations.h"

#include "QuickCutShared/Parsers/ProfileParser.h"

class ProfileManager : public BaseManager<Profile>, IParserOperations
{
public:
    ProfileManager() noexcept;
    ProfileManager(const QString & filePath) noexcept;
    ProfileManager(QString && filePath) noexcept;
    ~ProfileManager();

    bool load() override;
    bool save() override;

    bool            isLoadSucceed() override;
    const QString & getConfigFilePath() override;

    Profile * getActiveProfile();
    void      setActiveProfile(int index);
    void      setActiveProfile(Profile * profile);

private:
    ProfileParser m_Parser;
    bool          m_LoadSucceed;
};
