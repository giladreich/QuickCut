

#include "IParserOperations.h"
#include "Parser/PreferenceParser.h"

class PreferenceManager : IParserOperations
{
public:
    PreferenceManager() noexcept;
    PreferenceManager(const std::string & filePath) noexcept;
    PreferenceManager(std::string && filePath) noexcept;
    ~PreferenceManager();

    bool load() override;
    bool save() override;

    bool                isLoadSucceed() override;
    const std::string & getConfigFilePath() override;

    Preference & get();

private:
    Preference       m_Preference;
    PreferenceParser m_Parser;
    bool             m_LoadSucceed;
};