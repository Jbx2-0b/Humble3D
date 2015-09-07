#ifndef CPRIVATEXMLDATA_H
#define CPRIVATEXMLDATA_H

#include "qxmlputget.h"

//#include <QMap>
#include <QVariant>


//! @brief Classe réalisant le méchanisme de sérialisation xml
//! @author Jean-Baptiste Tymen
class CPrivateXmlData
{

public:

	//! Ctor
	CPrivateXmlData(const QString& tagName = "");

	//! Dtor	
	virtual ~CPrivateXmlData();

	//! Retourne le nom du tag
	virtual QString getTagName() const { return m_TagName; }

	//! Construit et retourne le xml sous forme de texte
	virtual QString toXmlString();

	//! Construit la classe en fonction d'un xml
	virtual void fromXmlString(const QString& strXml);

	//! Sauve le fichier xml
	virtual void saveXml(const QString& strFileName);

	//! Charge un fichier xml
	virtual void loadXml(const QString& strFileName);

	//! Vérifie si une clef existe
	virtual bool keyExist(const QString& key) const { return m_XmlDatas.contains(key); }

	//! Définit la valeur correspondant à la clé key
	virtual void setValue(const QString& key, QVariant value);

	//! Définit la valeur correspondant à la clé key
	virtual void setUserValue(const QString& key, const CPrivateXmlData& value);

	//! Définit l'ensemble des valeurs définies par la clef key
	virtual void removeValues(const QString& key);
	
	//! Retourne la valeur sous forme booléene correspondant à la clef key
	virtual bool getBoolValue(const QString& key) const { return m_XmlDatas[key].toBool(); }

	//! Retourne la valeur sous forme entiére correspondant à la clef key
	virtual int getIntValue(const QString& key) const { return m_XmlDatas[key].toInt(); }

	//! Retourne la valeur sous forme réelle correspondant à la clef key
	virtual double getDoubleValue(const QString& key) const { return m_XmlDatas[key].toDouble(); }

	//! Retourne la valeur sous forme chaîne de caractére correspondant à la clef key
	virtual QString getStringValue(const QString& key) const { return m_XmlDatas[key].toString(); }

	//! Retourne la valeur sous forme de date correspondant à la clef key
	virtual QTime getTimeValue(const QString& key) const { return m_XmlDatas[key].toTime(); }

	//! Retourne la valeur sous forme de date correspondant à la clef key
	virtual QDateTime getDateTimeValue(const QString& key) const { return m_XmlDatas[key].toDateTime(); }

	//! Retourne la valeur sous forme de liste de booleen correspondant à la clef key
	virtual QList<bool> getListBoolValue(const QString& key) const { return fromVariantList<bool>(m_XmlDatas[key].toList()); }

	//! Retourne la valeur sous forme de liste d'entiers correspondant à la clef key
	virtual QList<int> getListIntValue(const QString& key) const { return fromVariantList<int>(m_XmlDatas[key].toList()); }

	//! Retourne la valeur sous forme de liste de double correspondant à la clef key
	virtual QList<double> getListDoubleValue(const QString& key) const { return fromVariantList<double>(m_XmlDatas[key].toList()); }
	
protected:

	//! Ajoute un couple clef / valeur
	virtual void addValue(const QString& key, QVariant defaultValue);

	//! Ajoute un couple clef / valeur
	virtual void addValue(const QString& key, const CPrivateXmlData& xValue);

	//! Retourne la valeur correspondant à la clef key
	QVariant getValue(const QString& key) { return m_XmlDatas[key]; }

	//! Retourne la liste de valeur correspondant à la clef key
	QList<QVariant> getValues(const QString& key)  { return m_XmlDatas.values(key); }
	
	//! Ecrit le contenu de la classe sous forme de tring Xml
	virtual void write(QXmlPut* pXmlPut);

	//! Lit le contenu de la classe sous forme de tring Xml
	virtual void read(QXmlGet* pXmlGet);

	//! Conteneur associatif clef / valeur
	QMap<QString, QVariant> m_XmlDatas;

	//! Retourne le conteneur associatif clef / valeur
	QMap<QString, QVariant> getXmlData() { return m_XmlDatas; }

	//! Nom du tag
	QString m_TagName;

	//! Fonction template permettant la conversion d'une liste classique vers une liste de QVariant
	template <typename T>
	static QVariantList toVariantList(const QList<T>& list)
	{
		QVariantList newList;
		foreach (const T &item, list)
		{
			newList << item;
		}

		return newList;
	}

	//! Fonction template permettant la conversion d'une liste de QVariant vers une liste classique
	template <typename T>
	static QList<T> fromVariantList(const QVariantList& list)
	{
		QList<T> newList;
		foreach(const QVariant &item, list)
		{
			newList << item.value<T>();
		}

		return newList;
	}
};
Q_DECLARE_METATYPE(CPrivateXmlData)



#endif // CPRIVATEXMLDATA_H