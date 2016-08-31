#include "CPrivateXmlData.h"

//------------------------------------------------------------------------------
CPrivateXmlData::CPrivateXmlData(const QString& tagName /*= ""*/)
: m_TagName(tagName)
{
	qRegisterMetaType<CPrivateXmlData>("CPrivateXmlData");
}

//------------------------------------------------------------------------------
CPrivateXmlData::~CPrivateXmlData()
{
}

//------------------------------------------------------------------------------
void CPrivateXmlData::addValue(const QString& key, QVariant defaultValue)
{
	m_XmlDatas.insertMulti(key, defaultValue);
}

//------------------------------------------------------------------------------
void CPrivateXmlData::setValue(const QString& key, QVariant value)
{
	if (!m_XmlDatas.contains(key))
	{
		qDebug() << "CPrivateXmlData::setValue() : This key (" << key << ") has not been declared using declareXmlTags(). Value not set.";
	}
	else
	{
		m_XmlDatas[key] = value;
	}
}

//------------------------------------------------------------------------------
void CPrivateXmlData::setUserValue(const QString& key, const CPrivateXmlData& value)
{
	if (!m_XmlDatas.contains(key))
	{
		qDebug() << "CPrivateXmlData::setUserValue() : This key (" << key << ") has not been declared using declareXmlTags(). Value not set.";
	}
	else
	{
		m_XmlDatas[key] = QVariant::fromValue(value);
	}
}

//------------------------------------------------------------------------------
void CPrivateXmlData::removeValues(const QString& key)
{
	m_XmlDatas.remove(key);
}

//------------------------------------------------------------------------------
void CPrivateXmlData::addValue(const QString& key, const CPrivateXmlData& xValue)
{
	if (key.isEmpty())
	{
		qDebug() << "CPrivateXmlData::addValue() : Key null - not added !";
	}
	else
	{
		m_XmlDatas.insertMulti(key, QVariant::fromValue(xValue));
	}
}

//------------------------------------------------------------------------------
void CPrivateXmlData::write(QXmlPut* pXmlPut)
{
	// On écrit l'ensemble des types de base comme attribut
	QMapIterator<QString, QVariant> it(m_XmlDatas);

	while (it.hasNext())
	{
		it.next();

		switch (it.value().type())
		{
		case QVariant::Bool:
			pXmlPut->setAttributeBool(it.key(), it.value().toBool());
			break;

		case QVariant::Int:
			pXmlPut->setAttributeInt(it.key(), it.value().toInt());
			break;

		case QVariant::Double:
			pXmlPut->setAttributeDouble(it.key(), it.value().toDouble());
			break;

		case QVariant::String:
			pXmlPut->setAttributeString(it.key(), it.value().toString());
			break;

		case QVariant::DateTime:
			pXmlPut->setAttributeDateTime(it.key(), it.value().toDateTime());
			break;
		
		case QVariant::List:
		{
			QVariantList list = it.value().toList();
			
			if (list.count() > 0)
			{
				switch (list.first().type())
				{
					case QVariant::Bool:
						pXmlPut->setAttributeBoolVector(it.key(), fromVariantList<bool>(list).toVector());
						break;

					case QVariant::Int:
						pXmlPut->setAttributeIntVector(it.key(), fromVariantList<int>(list).toVector());
						break;
					
					case QVariant::Double:
						pXmlPut->setAttributeDoubleVector(it.key(), fromVariantList<double>(list).toVector());
						break;

					default:
						break;
				}
			}
		}
		break;

			
		default:
			break;
		}
	}

	// On écrit l'ensemble des user type dans des tags propres
	QMapIterator<QString, QVariant> itUT(m_XmlDatas);

	while (itUT.hasNext())
	{
		itUT.next();

        if (itUT.value().type() == QVariant::UserType)
        {
			CPrivateXmlData xmlData = itUT.value().value<CPrivateXmlData>();
			pXmlPut->descended(xmlData.getTagName());
			xmlData.write(pXmlPut);	
		}
	}
}

//------------------------------------------------------------------------------
void CPrivateXmlData::read(QXmlGet* pXmlGet)
{
	// On lit l'ensemble des types de base comme attribut
	QMapIterator<QString, QVariant> it(m_XmlDatas);

	while (it.hasNext())
	{
		it.next();

		switch (it.value().type())
		{
		case QVariant::Bool:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeBool(it.key());
			break;

		case QVariant::Int:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeInt(it.key());
			break;

		case QVariant::Double:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeDouble(it.key());
			break;

		case QVariant::String:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeString(it.key());
			break;

		case QVariant::DateTime:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeDateTime(it.key());
			break;

		case QVariant::List:
			m_XmlDatas[it.key()] = pXmlGet->getAttributeVariantList(it.key());
			break;

		default:
			break;
		}
	}

	// On écrit l'ensemble des user type dans des tags propres
	QMapIterator<QString, QVariant> itUT(m_XmlDatas);

	while (itUT.hasNext())
	{
		itUT.next();

        if (itUT.value().type() == QVariant::UserType)
        {
			CPrivateXmlData xmlData = itUT.value().value<CPrivateXmlData>();
			pXmlGet->find(itUT.key());
			xmlData.read(pXmlGet);
			break;
		}
	}

}

//------------------------------------------------------------------------------
QString CPrivateXmlData::toXmlString()
{
	QXmlPut xmlPut(getTagName());
	write(&xmlPut);
	return xmlPut.toString();
}

//------------------------------------------------------------------------------
void CPrivateXmlData::fromXmlString(const QString& strXml)
{
	QXmlGet xmlGet;
	xmlGet.fromString(strXml);
	read(&xmlGet);
}

//------------------------------------------------------------------------------
void CPrivateXmlData::saveXml(const QString& strFileName)
{
	QXmlPut xmlPut(getTagName());
	write(&xmlPut);
	xmlPut.save(strFileName);
}

//------------------------------------------------------------------------------
void CPrivateXmlData::loadXml(const QString& strFileName)
{
	QXmlGet xmlGet;
	xmlGet.load(strFileName);
	read(&xmlGet);
}
