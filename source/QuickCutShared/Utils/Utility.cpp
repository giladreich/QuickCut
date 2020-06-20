
#include "pch.h"
#include "Utils/Utility.h"

namespace boost
{
namespace property_tree
{
    void write_jsonEx(const std::string & path, const JSON & ptree)
    {
        std::ostringstream oss;
        bpt::write_json(oss, ptree);
        std::regex  reg("\\\"([0-9]+\\.{0,1}[0-9]*)\\\"");
        std::string result = std::regex_replace(oss.str(), reg, "$1");

        std::ofstream file;
        file.open(path);
        file << result;
        file.close();
    }
} // namespace property_tree
} // namespace boost

namespace Hook
{
#if defined(Q_OS_WIN)
void sendReloadSignal()
{
    const int INPUT_COUNT     = 4;
    int       vkKey1          = std::strtol("86", nullptr, 16);
    int       vkKey2          = std::strtol("87", nullptr, 16);
    INPUT     in[INPUT_COUNT] = {0};
    for (int i = 0; i < INPUT_COUNT; i++)
    {
        in[i].type       = INPUT_KEYBOARD;
        in[i].ki.dwFlags = KEYEVENTF_UNICODE;
        in[i].ki.wVk     = (i % 2 == 0) ? vkKey1 : vkKey2;
    }
    SendInput(INPUT_COUNT, in, sizeof(INPUT));

    // Sending key up, which should reset the key codes pattern and profiles has been reloaded.
    INPUT inUp      = {0};
    inUp.type       = INPUT_KEYBOARD;
    inUp.ki.dwFlags = KEYEVENTF_KEYUP;
    inUp.ki.wVk     = vkKey1;
    SendInput(1, &inUp, sizeof(INPUT));
}
#elif defined(Q_OS_UNIX)
void sendReloadSignal()
{
    // TODO(Gilad): Implement.
}
#endif
} // namespace Hook

std::string getDateTime()
{
    return QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate).toStdString();
}

std::string createUuid()
{
    return QUuid::createUuid().toString().toStdString();
}
