

#include "IParserOperations.h"

#include "QuickCutShared/Parsers/PreferenceParser.h"

class PreferenceManager : IParserOperations
{
public:
    PreferenceManager() noexcept;
    PreferenceManager(const QString & filePath) noexcept;
    PreferenceManager(QString && filePath) noexcept;
    ~PreferenceManager();

    bool save() override;
    bool load() override;

    bool            isLoadSucceed() override;
    const QString & getConfigFilePath() override;

    Preference &       get();
    const Preference & get() const;

private:
    Preference       m_Preference;
    PreferenceParser m_Parser;
    bool             m_LoadSucceed;
};
