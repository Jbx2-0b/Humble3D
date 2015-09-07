#ifndef CXMLREADER_H
#define CXMLREADER_H

// Lib
#include "lib_xml_global.h"

#include "CXmlElement.h"

// Qt
#include <QtXml>
#include <QString>

class LIB_XML_EXPORT CXmlReader
{
public:

	//! Constructeur
    CXmlReader(bool m_bDebugMode = false);

	//! Destructeur
    ~CXmlReader();

	//! Parse un fichier de config xml
    bool loadFile(const QString& fileName);

    //! Retourne l'élement root
    CXmlElement* getRootElement() { return m_pRootElement; }

	//! Retourne la valeur d'un attribut
	template<class T> T getAttributeValue(
            const QString& path,
			const T& defaultValue = T()) const
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
                // qDebug() << "Warning, Element not found: " << elementName << " Using default value : " << defaultValue;
				return defaultValue;
			}
		}

		return currentElement->getAttributeValue<T>(results.first());
	}

    //! Retourne les elements en fonction d'un chemin
    QList<CXmlElement*> getElements(const QString& path) const;

    //! Retourne un element en fonction d'un chemin
    CXmlElement* getElement(const QString& path) const;

	//! Retourne vrai si contient le chemin
    bool contains(const QString& path) const;

private:

    bool m_bDebugMode;

    CXmlElement* m_pRootElement;

    void recursiveParse(QDomElement* element, CXmlElement* parent);

    void parseAttributes(QDomElement* domElement, CXmlElement* element);
};

#endif // CXMLREADER_H

