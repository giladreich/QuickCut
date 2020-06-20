#include "pch.h"
#include "BaseModel.h"
#include "Utils/Utility.h"

// TODO(Gilad): Manage lastModified differently after finishing refactoring.

BaseModel::BaseModel() noexcept
    : m_Uuid(std::move(createUuid()))
    , m_Name()
    , m_LastModified(std::move(getDateTime()))
{
}

BaseModel::BaseModel(std::string && id,
                     std::string && name,
                     std::string && lastModified) noexcept
    : m_Uuid(std::move(id))
    , m_Name(std::move(name))
    , m_LastModified(std::move(lastModified))
{
}

BaseModel::BaseModel(const std::string & id,
                     const std::string & name,
                     const std::string & lastModified) noexcept
    : m_Uuid(id)
    , m_Name(name)
    , m_LastModified(lastModified)
{
}

const std::string & BaseModel::getId() const
{
    return m_Uuid;
}

const std::string & BaseModel::getName() const
{
    return m_Name;
}

void BaseModel::setName(const std::string & name)
{
    m_Name = name;
}

const std::string & BaseModel::getLastModified() const
{
    return m_LastModified;
}
