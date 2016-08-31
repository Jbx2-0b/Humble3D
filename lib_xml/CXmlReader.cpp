// App
#include "CXmlReader.h"

// Qt
#include <QDomNodeList>

CXmlReader::CXmlReader(bool debugMode_ /*= false*/)
: m_bDebugMode(debugMode_)
, m_pRootElement(0)
{
}

CXmlReader::~CXmlReader()
{
    delete m_pRootElement;
}

bool CXmlReader::loadFile(const QString& xmlFileName)
{
    delete m_pRootElement;
    m_pRootElement = 0;

	QDomDocument doc("mydocument");
	QFile file(xmlFileName);
	if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error " + xmlFileName + " not found !";
		return false;
    }

    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
	{
        qDebug() << "Warning - Error in xml file: " << xmlFileName;
        qDebug() << errorMsg << " Line: " << errorLine << " Column: " << errorColumn;
        return false;
	}
	
	file.close();

	
    // Get the top-level element: Name is "root". No attributes for "root"
	QDomElement qRootElement = doc.documentElement();
    m_pRootElement = new CXmlElement("root", 0, m_bDebugMode);
    recursiveParse(&qRootElement, m_pRootElement);

    return true;
}

QList<CXmlElement*> CXmlReader::getElements(const QString& path) const
{
	QList<QString> results = path.split(".", QString::SkipEmptyParts);

    CXmlElement* currentElement = m_pRootElement;

    // On parcourt l'arbre des elements, sauf le dernier qui correspond a l'attribut
	while (results.size() > 1)
	{
		QString elementName = results.first();

        if (CXmlElement* child = currentElement->getChildByName(elementName))
		{
			currentElement = child;
			results.removeFirst();
		}
		else
		{
			qDebug() << "Warning, Element not found: " << elementName;
            return QList<CXmlElement*>();
		}
	}

	return currentElement->getChildElementsByName(results.first());
}


CXmlElement* CXmlReader::getElement(const QString& path) const
{
    QList<CXmlElement*> elements = getElements(path);

    if (elements.count() > 0)
        return elements.first();

    return 0;
}

bool CXmlReader::contains(const QString& path) const
{
	QList<QString> results = path.split(".", QString::SkipEmptyParts);

    CXmlElement* currentElement = m_pRootElement;

	while (results.size() > 1)
	{
		QString elementName = results.first();

        if (CXmlElement* child = currentElement->getChildByName(elementName))
		{
			currentElement = child;
			results.removeFirst();
		}
		else
		{
			qDebug() << "Warning, Element not found: " << elementName;
			return false;
		}
	}

	return currentElement->contains(results.front());
}

void CXmlReader::recursiveParse(QDomElement* domElement, CXmlElement* element)
{
    // Parse XML file for tags of interest
    QDomNodeList children = domElement->childNodes();

    // For all nodes, children of "root" in the XML tree.
    for (int i = 0; i < children.length(); ++i)
	{
        const QDomNode& currentNode = children.at(i);

		// is element ?
        if (currentNode.nodeType() == QDomNode::ElementNode)
		{
            QString nodeName = currentNode.nodeName();
            CXmlElement* pChild = element->createChild(nodeName);

			// Found node which is an Element. Re-cast node as element
			QDomElement currentDomElement = currentNode.toElement();

            parseAttributes(&currentDomElement, pChild);
            recursiveParse(&currentDomElement, pChild);
            pChild->setValue(currentNode.firstChild().nodeValue());
        }
	}
}

void CXmlReader::parseAttributes(QDomElement* domElement, CXmlElement* element)
{
    QDomNamedNodeMap attributes = domElement->attributes();

    for (int i = 0; i < attributes.length(); ++i)
    {
        QDomNode currentNode = attributes.item(i);

		// is attribute ?
        if(currentNode.nodeType() == QDomNode::AttributeNode)
		{
			// Found node which is an Attribute. Re-cast node as attribute
            QDomAttr attribute = currentNode.toAttr();

			element->addAttribute(attribute.name(), attribute.value());
		}
    }
}

