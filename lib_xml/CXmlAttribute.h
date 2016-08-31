/*
 * XmlAttribute.h
 *
 *  Created on: Oct 14, 2014
 *      Author: jt101632
 */

#ifndef CXMLATTRIBUTE_H_
#define CXMLATTRIBUTE_H_

// Qt
#include <QString>

class CXmlAttribute
{
public:
    CXmlAttribute(const QString& name, const QString& value);

    QString getName() const;

    QString getValue() const;

private:
    QString m_Name;

    QString m_Value;
};

#endif // CXMLATTRIBUTE_H_
