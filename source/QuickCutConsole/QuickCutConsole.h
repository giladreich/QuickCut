
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
    static void executeProcess(const std::string & szProc, const std::string & szArgs);
    static void log(const QString & szFilePath, const QString & szMessage);

public:
    static QuickCutConsole *              s_pInstance;
    static std::unique_ptr<class Profile> s_pProfile;
    static std::string                    s_qszProfilesPath;
};
