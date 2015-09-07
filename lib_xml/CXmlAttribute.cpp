#include "CXmlAttribute.h"

CXmlAttribute::CXmlAttribute(const QString& name, const QString& value)
    : m_Name(name)
    , m_Value(value)
{

}

QString CXmlAttribute::getName() const
{
    return m_Name;
}

QString CXmlAttribute::getValue() const
{
    return m_Value;
}
