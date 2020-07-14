
#pragma once

#include "QuickCutShared/QuickCutPCH.h"

class BaseModel
{
protected:
    BaseModel() noexcept;
    BaseModel(const QString & id, const QString & lastModified) noexcept;
    BaseModel(QString && id, QString && lastModified) noexcept;

    virtual ~BaseModel() = default;

public:
    const QString & getId() const;

    const QString & getName() const;
    void            setName(const QString & name);

    const QString & getLastModified() const;

protected:
    QString m_Uuid;
    QString m_Name;
    QString m_LastModified;
};
