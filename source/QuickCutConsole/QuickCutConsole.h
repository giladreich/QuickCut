
#pragma once

#include "QuickCutShared/QuickCutPCH.h"
#include "QuickCutShared/Managers/ProfileManager.h"

#if defined(Q_OS_WINDOWS)
#    include "QuickCutShared/Hooks/KeyboardHookWindows.h"
#else
#    include "QuickCutShared/Hooks/KeyboardHookUnix.h"
#endif

#include <QCoreApplication>

class QLocalServer;

class QuickCutConsole : public QCoreApplication
{

public:
    QuickCutConsole(int argc, char * argv[]);
    virtual ~QuickCutConsole();

    virtual bool start();
    virtual bool stop();
    virtual void sendInput(const KeyboardKeys & dstKeys, KeyboardHook::KeyEvent keyEvent) = 0;
    virtual void executeProcess(const QString & process, const QString & arguments)       = 0;

    bool loadProfiles();
    bool notifyStatusToClient(const QString & message);

public slots:
    void onKeysPress(const KeyboardKeys & keys, bool * outSwallowKey);
    void onKeysDown(const KeyboardKeys & keys, bool * outSwallowKey);
    void onKeyUp(const KeyData & key, bool * outSwallowKey);

protected:
    static QuickCutConsole * s_Instance;

    std::shared_ptr<Profile> m_Profile;
    ProfileManager           m_ProfileManager;

    // Will be used for signaling when the GUI updates the profiles.json file, the server will
    // reload the profiles in order to get things synchronized.
    QPointer<QLocalServer> m_LocalSocket;
    QPointer<KeyboardHook> m_Hook;
};
