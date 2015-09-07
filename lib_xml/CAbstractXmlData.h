#ifndef CABSTRACTXMLDATA_H
#define CABSTRACTXMLDATA_H

#include "CPrivateXmlData.h"


//! @brief Classe de base pour la d�finition d'une classe s�rializable en xml
//! @author Jean-Baptiste Tymen
class CAbstractXmlData : protected CPrivateXmlData
{
public:

	//! Ctor
	CAbstractXmlData(const QString& tagName) : CPrivateXmlData(tagName)	{ m_bTagDeclared = false; }

	//! Construit et retourne le xml sous forme de texte
	virtual QString toXmlString() { verifyTagDeclaration(); return CPrivateXmlData::toXmlString(); }

	//! Construit la classe en fonction d'un xml
	virtual void fromXmlString(const QString& strXml) { verifyTagDeclaration(); CPrivateXmlData::fromXmlString(strXml); }

	//! Sauve le fichier xml
	virtual void saveXml(const QString& strFileName) { verifyTagDeclaration(); CPrivateXmlData::saveXml(strFileName); }

	//! Charge un fichier xml
	virtual void loadXml(const QString& strFileName) { verifyTagDeclaration(); CPrivateXmlData::loadXml(strFileName); }

	//! V�rifie si une clef existe
	virtual bool keyExist(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::keyExist(key); }

	//! D�finit la valeur correspondant � la cl� key
	virtual void setValue(const QString& key, QVariant value) { verifyTagDeclaration(); CPrivateXmlData::setValue(key, value); }

	//! D�finit la valeur correspondant � la cl� key
	virtual void setUserValue(const QString& key, const CAbstractXmlData& value) { verifyTagDeclaration(); CPrivateXmlData::setUserValue(key, value); }

	//! D�finit l'ensemble des valeurs d�finies par la clef key
	virtual void removeValues(const QString& key) { verifyTagDeclaration(); CPrivateXmlData::removeValues(key); }

	//! Retourne la valeur correspondant � la clef key
	template<typename T> T getValue(const QString& key) { verifyTagDeclaration(); return m_XmlDatas[key].value<T>(); }

	//! Retourne la liste de valeur correspondant � la clef key
	template<typename T> QList<T> getValues(const QString& key);
	
	//! Retourne la valeur sous forme bool�ene correspondant � la clef key
	virtual bool getBoolValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getBoolValue(key); }

	//! Retourne la valeur sous forme enti�re correspondant � la clef key
	virtual int getIntValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getIntValue(key); }

	//! Retourne la valeur sous forme r�elle correspondant � la clef key
	virtual double getDoubleValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getDoubleValue(key); }

	//! Retourne la valeur sous forme cha�ne de caract�re correspondant � la clef key
	virtual QString getStringValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getStringValue(key); }

	//! Retourne la valeur sous forme de temps correspondant � la clef key
	virtual QTime getTimeValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getTimeValue(key); }

	//! Retourne la valeur sous forme de date correspondant � la clef key
	virtual QDateTime getDateTimeValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getDateTimeValue(key); }

	//! Retourne la valeur sous forme de liste de booleens correspondant � la clef key
	virtual QList<bool> getListBoolValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getListBoolValue(key); }

	//! Retourne la valeur sous forme de liste d'entiers correspondant � la clef key
	virtual QList<int> getListIntValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getListIntValue(key); }

	//! Retourne la valeur sous forme de liste de double correspondant � la clef key
	virtual QList<double> getListDoubleValue(const QString& key) { verifyTagDeclaration(); return CPrivateXmlData::getListDoubleValue(key); }


protected:

	virtual void declareXmlTags() = 0;

private:

	//! S'assure que les tagName ont �t� d�clar�s. La fonction �tant virtuelle (pure m�me...), on peut pas simplement la passer dans le constructeur ;)
	void verifyTagDeclaration()
	{
		if (!m_bTagDeclared)
		{
			declareXmlTags();
			m_bTagDeclared = true;
		}
	}

	bool m_bTagDeclared;
};

//-------------------------------------------------------------------------------------------------
template<typename T> QList<T> CAbstractXmlData::getValues(const QString& key)
{
	verifyTagDeclaration();

	QList<T> vList;

	foreach (const QVariant& v, m_XmlDatas.values(key))
	{
		vList.append(v.value<T>());
	}

	return vList;
}

#endif // CABSTRACTXMLDATA_H