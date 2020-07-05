
#pragma once

#include "QuickCutShared/QuickCutPCH.h"

#include <QCoreApplication>

class Profile;
class ProfileManager;
class QLocalServer;

class QuickCutConsole : public QCoreApplication
{

protected:
    QuickCutConsole(int argc, char * argv[]);
    virtual ~QuickCutConsole();

    virtual bool start();
    virtual bool stop();

    static bool loadProfiles();
    static void executeProcess(const QString & process, const QString & arguments);
    static void log(const QString & filePath, const QString & text);

    static bool notifyStatusToClient(const QString & message);

protected:
    static QuickCutConsole * s_Instance;
    static Profile *         s_Profile;

    // Will be used for signaling when the GUI updates the profiles.json file, the server will
    // reload the profiles in order to get things synchronized.
    static QLocalServer * s_LocalSocket;

private:
    static ProfileManager s_ProfileManager;
};
