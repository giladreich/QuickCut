
#pragma once

#include "QtService/QtService"

class QuickCutService : public QtService<QCoreApplication>
{
public:
    QuickCutService(int argc, char * argv[]);
    virtual ~QuickCutService();

    virtual void start() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void stop() override;

    virtual bool isProcessRunning(const QString & szProc);

    static void log(const QString & szFilePath, const QString & szMessage);

public:
};
