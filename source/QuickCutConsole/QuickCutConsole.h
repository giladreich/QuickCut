
#pragma once

#include "pch.h"
#include <QCoreApplication>

class Profile;
class ProfileManager;

class QuickCutConsole : public QCoreApplication
{

public:
    QuickCutConsole(int argc, char * argv[]);
    virtual ~QuickCutConsole();

    virtual bool start();
    virtual bool stop();

    static bool loadProfiles();
    static void executeProcess(const QString & process, const QString & arguments);
    static void log(const QString & filePath, const QString & text);

public:
    static QuickCutConsole * s_Instance;
    static Profile *         s_Profile;

private:
    static ProfileManager s_ProfileManager;
};
