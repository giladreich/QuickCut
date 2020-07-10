
#pragma once

#include "QuickCutShared/QtService/QtService"

#include <QObject>

class QuickCutService : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT

public:
    QuickCutService(int argc, char * argv[]);
    virtual ~QuickCutService();

    virtual void start() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void resume() override;

    virtual bool startHook()     = 0;
    virtual bool isHookRunning() = 0;

public slots:
    void verifyHookRunning();

protected:
    QTimer m_Timer;
    int    m_CheckInterval;
};
