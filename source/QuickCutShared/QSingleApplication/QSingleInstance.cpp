
#include "pch.h"
#include "QSingleInstance.h"

#include <QCryptographicHash>

namespace
{

    QString generateKeyHash(const QString & szKey, const QString & szSalt)
    {
        QByteArray data;

        data.append(szKey.toUtf8());
        data.append(szSalt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
    }

} // namespace

QSingleInstance::QSingleInstance(const QString & szKey)
    : m_szKey(szKey)
    , m_szMemLockKey(generateKeyHash(szKey, "_memLockKey"))
    , m_szSharedMemKey(generateKeyHash(szKey, "_sharedmemKey"))
    , m_qSharedMem(m_szSharedMemKey)
    , m_qMemLock(m_szMemLockKey, 1)
{
    m_qMemLock.acquire();
    {
        QSharedMemory fix(m_szSharedMemKey); // Fix for *nix: http://habrahabr.ru/post/173281/
        fix.attach();
    }
    m_qMemLock.release();
}

QSingleInstance::~QSingleInstance()
{
    release();
}

bool QSingleInstance::isAnotherRunning()
{
    if (m_qSharedMem.isAttached()) return false;

    m_qMemLock.acquire();
    const bool isRunning = m_qSharedMem.attach();
    if (isRunning) m_qSharedMem.detach();
    m_qMemLock.release();

    return isRunning;
}

bool QSingleInstance::tryToRun()
{
    if (isAnotherRunning()) // Extra check
        return false;

    m_qMemLock.acquire();
    const bool result = m_qSharedMem.create(sizeof(quint64));
    m_qMemLock.release();
    if (!result)
    {
        release();
        return false;
    }

    return true;
}

void QSingleInstance::release()
{
    m_qMemLock.acquire();
    if (m_qSharedMem.isAttached()) m_qSharedMem.detach();
    m_qMemLock.release();
}