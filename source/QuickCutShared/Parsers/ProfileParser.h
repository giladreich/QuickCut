
#pragma once

#include "BaseParser.h"

#include "QuickCutShared/Models/Profile.h"

class ProfileParser : public BaseParser<std::vector<std::shared_ptr<Profile>>>
{
public:
    ProfileParser(QString && path);
    ProfileParser(const QString & path);
    virtual ~ProfileParser() = default;

private:
    bool saveImpl(const std::vector<std::shared_ptr<Profile>> & data) override;
    bool parseImpl(std::vector<std::shared_ptr<Profile>> * outData) override;

    std::pair<std::wstring, JSON> serializeKeys(const std::wstring & key,
                                                const KeyboardKeys & keys);
    KeyboardKeys                  deserializeKeys(const std::wstring & key, JSON actionJson);
};
