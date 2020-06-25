

#include "IParserOperations.h"
#include "Parsers/PreferenceParser.h"

class PreferenceManager : IParserOperations
{
public:
    PreferenceManager() noexcept;
    PreferenceManager(const QString & filePath) noexcept;
    PreferenceManager(QString && filePath) noexcept;
    ~PreferenceManager();

    bool load() override;
    bool save() override;

    bool            isLoadSucceed() override;
    const QString & getConfigFilePath() override;

    Preference & get();

private:
    Preference       m_Preference;
    PreferenceParser m_Parser;
    bool             m_LoadSucceed;
};