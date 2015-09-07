/*
 * XmlWriter.cc
 *
 *  Created on: Oct 14, 2014
 *      Author: jt101632
 */

#include "CXmlWriter.h"

bool CXmlWriter::saveFile(const QString& fileName, const CXmlElement* pRootElement)
{
    QDomDocument doc("mydocument");
    QDomElement rootElement = doc.createElement(pRootElement->getName());
    doc.appendChild(rootElement);

    foreach (const CXmlElement* pXmlElement, pRootElement->getChildElements())
    {
        recursiveAppendElements(&doc, &rootElement, pXmlElement);
    }

    QFile outFile(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug( "CXmlWriter : Failed to open file for writing." );
        return false;
    }

    QTextStream stream(&outFile);
    stream << doc.toString();
    outFile.close();

    return true;
}

void CXmlWriter::recursiveAppendElements(
        QDomDocument* pDocument,
        QDomElement* pParentElement,
        const CXmlElement* pXmlElement)
{
    QDomElement element = pDocument->createElement(pXmlElement->getName());

    QMapIterator<QString, QString> it(pXmlElement->getAttributes());
    while (it.hasNext())
    {
        it.next();
        element.setAttribute(it.key(), it.value());
    }

    pParentElement->appendChild(element);

    foreach (const CXmlElement* xmlChildElement, pXmlElement->getChildElements())
    {
        recursiveAppendElements(pDocument, &element, xmlChildElement);
    }
}
