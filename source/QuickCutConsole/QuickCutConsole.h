
#pragma once

#include <QCoreApplication>
#include <memory>

class QuickCutConsole : public QCoreApplication
{

public:
    QuickCutConsole(int argc, char * argv[]);
    virtual ~QuickCutConsole();

    virtual bool start();
    virtual bool stop();

    static bool loadProfiles();
    static void executeProcess(const std::string & process, const std::string & arguments);
    static void log(const QString & filePath, const QString & text);

public:
    static QuickCutConsole *              s_Instance;
    static std::unique_ptr<class Profile> s_Profile;
    static std::string                    s_ProfilesPath;
};
