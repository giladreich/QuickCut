
#pragma once

#include "QuickCutShared/QuickCutPCH.h"

class BaseModel
{
protected:
    BaseModel() noexcept;
    BaseModel(const QString & id,
              const QString & lastModified,
              const QString & createdDate) noexcept;
    BaseModel(QString && id, QString && lastModified, QString && createdDate) noexcept;

    virtual ~BaseModel() = default;

public:
    const QString & getId() const;

    const QString & getName() const;
    void            setName(const QString & name);

    const QString & getLastModified() const;
    const QString & getCreatedDate() const;

protected:
    QString m_Uuid;
    QString m_Name;
    QString m_LastModified;
    QString m_CreatedDate;
};
