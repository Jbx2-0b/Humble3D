/*
 * XmlWriter.h
 *
 *  Created on: Oct 14, 2014
 *      Author: jt101632
 */

#ifndef XMLWRITER_H_
#define XMLWRITER_H_

// Lib
#include "lib_xml_global.h"

#include "CXmlElement.h"

// Qt
#include <QtXml>

class LIB_XML_EXPORT CXmlWriter
{
public:

    static bool saveFile(const QString& fileName, const CXmlElement* pRootElement);

private:
    static void recursiveAppendElements(
            QDomDocument* pDocument,
            QDomElement* pParentElement,
            const CXmlElement* pXmlElement);
};

#endif /* XMLWRITER_H_ */
