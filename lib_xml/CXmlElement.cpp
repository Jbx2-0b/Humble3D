#include "CXmlElement.h"

//-----------------------------------------------------------------------------------------
CXmlElement::CXmlElement(const QString& name, CXmlElement* parent, bool debugMode /* = false*/)
: m_Parent(parent)
, m_bDebugMode(debugMode)
, m_Name(name)
{
}

//-----------------------------------------------------------------------------------------
CXmlElement::~CXmlElement()
{
	// On supprime l'ensemble des enfants
    foreach (CXmlElement* element, m_ChildElements)
	{
		delete element;
	}

    m_ChildElements.clear();
}

//-----------------------------------------------------------------------------------------
QString CXmlElement::getName() const
{
	return m_Name;
}

//-----------------------------------------------------------------------------------------
QString CXmlElement::getValue() const
{
    return m_Value;
}

//-----------------------------------------------------------------------------------------
void CXmlElement::setValue(const QString& value)
{
    m_Value = value;
}

//-----------------------------------------------------------------------------------------
QString CXmlElement::getPath() const
{
	QString path = m_Name;

    CXmlElement* current = getParent();

	while (current)
	{
		path.insert(0, current->getName() + ".");
        current = current->getParent();
	}

	return path;
}

//-----------------------------------------------------------------------------------------
unsigned int CXmlElement::getDepth() const
{
    unsigned int uiLevel = 0;

    const CXmlElement* pCurrent = this;

    while (pCurrent != 0)
    {
        uiLevel++;
        pCurrent = pCurrent->getParent();
    }

    return uiLevel;
}

//-----------------------------------------------------------------------------------------
CXmlElement* CXmlElement::getParent() const
{
    return m_Parent;
}

//-----------------------------------------------------------------------------------------
void CXmlElement::addAttribute(const QString& name, const QString& value)
{
    m_Attributes[name] = value;
}

//-----------------------------------------------------------------------------------------
void CXmlElement::addAttribute(const QString& name, const QVector2D& value)
{
    addAttribute(name, QString("%1;%2").arg(value.x()).arg(value.y()));
}

//-----------------------------------------------------------------------------------------
void CXmlElement::addAttribute(const QString& name, const QVector3D& value)
{
    addAttribute(name, QString("%1;%2;%3").arg(value.x()).arg(value.y()).arg(value.z()));
}

//-----------------------------------------------------------------------------------------
void CXmlElement::addAttribute(const QString& name, const QVector4D& value)
{
    addAttribute(name, QString("%1;%2;%3;%4").arg(value.x()).arg(value.y()).arg(value.z()).arg(value.w()));
}

//-----------------------------------------------------------------------------------------
bool CXmlElement::contains(const QString& attributeName) const
{
	return m_Attributes.contains(attributeName);
}

//-----------------------------------------------------------------------------------------
CXmlElement* CXmlElement::createChild(const QString& name)
{
    CXmlElement* pChild = new CXmlElement(name, this, m_bDebugMode);
    m_ChildElements << pChild;
    return pChild;
}

//-----------------------------------------------------------------------------------------
const QList<CXmlElement*>& CXmlElement::getChildElements() const
{
    return m_ChildElements;
}

//-----------------------------------------------------------------------------------------
CXmlElement* CXmlElement::getChildByName(const QString& name) const
{
    foreach (CXmlElement* element, m_ChildElements)
	{
        if (element->getName() == name)
		{
			return element;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------------------
QList<CXmlElement*> CXmlElement::getChildElementsByName(const QString& name) const
{
    QList<CXmlElement*> elements;
    foreach (CXmlElement* element, m_ChildElements)
	{
		if (element->getName() == name)
		{
			elements << element;
		}
	}

	return elements;
}

//-----------------------------------------------------------------------------------------
void CXmlElement::dumpNodeTree() const
{
    recursiveDump(0);
}

//-----------------------------------------------------------------------------------------
void CXmlElement::recursiveDump(int iLevel) const
{
    QString branchNode = "|";
    QString branchItems = " ";

    for (int i = 0; i < iLevel; ++i)
    {
        branchNode += "-";
        branchItems += " ";
    }
    qDebug() << branchNode << "Node name : " << getName();

    foreach (CXmlElement* pChild, m_ChildElements)
    {
        pChild->recursiveDump(iLevel + 1);
    }
}



