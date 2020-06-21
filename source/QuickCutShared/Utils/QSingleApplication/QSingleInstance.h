/*
 * https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection
 */
#pragma once

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class QSingleInstance
{
    Q_DISABLE_COPY(QSingleInstance)

public:
    QSingleInstance(const QString & szKey);
    ~QSingleInstance();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString m_szKey;
    const QString m_szMemLockKey;
    const QString m_szSharedMemKey;

    QSharedMemory    m_qSharedMem;
    QSystemSemaphore m_qMemLock;
};
