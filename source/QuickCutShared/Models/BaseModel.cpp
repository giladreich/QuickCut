
#include "QuickCutShared/QuickCutPCH.h"
#include "BaseModel.h"

BaseModel::BaseModel() noexcept
    : m_Uuid(std::move(QuickCut::createUuid()))
    , m_Name()
    , m_LastModified(std::move(QuickCut::getDateTime()))
    , m_CreatedDate(std::move(QuickCut::getDateTime()))
{
}

BaseModel::BaseModel(const QString & id,
                     const QString & lastModified,
                     const QString & createdDate) noexcept
    : m_Uuid(id)
    , m_Name()
    , m_LastModified(lastModified)
    , m_CreatedDate(createdDate)
{
}

BaseModel::BaseModel(QString && id, QString && lastModified, QString && createdDate) noexcept
    : m_Uuid(std::move(id))
    , m_Name()
    , m_LastModified(std::move(lastModified))
    , m_CreatedDate(std::move(createdDate))
{
}

const QString & BaseModel::getId() const
{
    return m_Uuid;
}

const QString & BaseModel::getName() const
{
    return m_Name;
}

void BaseModel::setName(const QString & name)
{
    m_Name         = name;
    m_LastModified = QuickCut::getDateTime();
}

const QString & BaseModel::getLastModified() const
{
    return m_LastModified;
}

const QString & BaseModel::getCreatedDate() const
{
    return m_CreatedDate;
}
