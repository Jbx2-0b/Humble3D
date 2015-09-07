#ifndef XMLELEMENT_H
#define XMLELEMENT_H

// Lib
#include "lib_xml_global.h"

// Qt
#include <QString>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

class LIB_XML_EXPORT CXmlElement
{
public:

    //! Constructeur
    CXmlElement(const QString& name, CXmlElement* parent = 0, bool bDebugMode = false);

    //! Destructeur
    ~CXmlElement();

    //! Retourne le nom du tag
    QString getName() const;

    //! Retourne le texte de l'element
    QString getValue() const;

    //! Définit le texte de l'element
    void setValue(const QString& value);

    //! Retourne le chemin de l'element
    QString getPath() const;

    //! Retourne la profondeur du noeud
    unsigned int getDepth() const;

    //! Retourne le parent de l'element
    CXmlElement* getParent() const;

    //! Ajoute un attribut au noeud
    void addAttribute(const QString& name, const QString& value);

    //! Ajoute un attribut au noeud
    template<class T> inline void addAttribute(const QString& name, const T& value);

    //! Ajoute un attribut au noeud
    void addAttribute(const QString& name, const QVector2D& value);

    //! Ajoute un attribut au noeud
    void addAttribute(const QString& name, const QVector3D& value);

    //! Ajoute un attribut au noeud
    void addAttribute(const QString& name, const QVector4D& value);


    //! Retourne la valeur d'un attribut
    template<class T> inline T getAttributeValue(
            const QString& attributeName,
            const T& defaultValue = T()) const;

    //! Retourne l'ensemble des attributs
    const QMap<QString, QString>& getAttributes() const { return m_Attributes; }

    //! Retourne vrai si l'attribut est present
    bool contains(const QString& attributeName) const;

    //! Crée un element enfant
    CXmlElement* createChild(const QString& name);

    //! Retourne les elements enfants
    const QList<CXmlElement*>& getChildElements() const;

    //! Retourne un enfant en fonction de son nom
    CXmlElement* getChildByName(const QString& m_Name) const;

    //! Retourne des enfants en fonction de leur nom
    QList<CXmlElement*> getChildElementsByName(const QString& m_Name) const;

    //! Affiche l'arbre des noeuds sur la console
    void dumpNodeTree() const;

private:

    //! Parcourt et affiche l'arbre des noeuds sur la console
    void recursiveDump(int iLevel) const;

    CXmlElement* m_Parent;

    bool m_bDebugMode;
    QString m_Name;
    QString m_Value;
    QMap<QString, QString> m_Attributes;
    QList<CXmlElement*> m_ChildElements;
};

//! Retourne la valeur d'un attribut
template<class T> T CXmlElement::getAttributeValue(
        const QString& attributeName,
        const T& defaultValue) const
{
    if (m_Attributes.contains(attributeName))
    {
        T attribute = QVariant(m_Attributes.value(attributeName)).value<T>();

        if (m_bDebugMode)
        {
            qDebug() << getPath() << "." << attributeName << " = " << attribute;
        }

        return attribute;
    }

    //qDebug() << "Warning, Attribute not found: " << attributeName;

    return defaultValue;
}

//! Retourne la valeur d'un attribut - Specialisation bool
template<> inline bool CXmlElement::getAttributeValue<bool>(
        const QString& attributeName,
        const bool& defaultValue) const
{
    if (m_Attributes.find(attributeName) != m_Attributes.end())
    {
        bool attribute = defaultValue;
        QString sAttribute = m_Attributes[attributeName];

        if (sAttribute == "false")
            attribute = false;
        else if (sAttribute == "true")
            attribute = true;
        else
            attribute = (bool)m_Attributes[attributeName].toInt();

        if (m_bDebugMode)
        {
            qDebug() << getPath() << "." << attributeName << " = " << attribute;
        }

        return attribute;
    }

    //qDebug() << "Warning, Attribute not found: " << attributeName;

    return defaultValue;
}

//! Retourne la valeur d'un attribut - Specialisation Vector 2D
template<> inline QVector2D CXmlElement::getAttributeValue<QVector2D>(
        const QString& attributeName,
        const QVector2D& defaultValue) const
{
    if (m_Attributes.find(attributeName) != m_Attributes.end())
    {
        QVector2D attribute = defaultValue;
        QString sAttribute = m_Attributes[attributeName];

        QStringList strList = sAttribute.split(";");

        if (strList.size() == 2)
        {
            attribute.setX(strList[0].toFloat());
            attribute.setY(strList[1].toFloat());
        }
        else
        {
            qDebug() << "Erreur getAttributeValue<QVector2D>() : " << attributeName;
        }

        if (m_bDebugMode)
        {
            qDebug() << getPath() << "." << attributeName << " = " << attribute;
        }

        return attribute;
    }

    //qDebug() << "Warning, Attribute not found: " << attributeName;

    return defaultValue;
}

//! Retourne la valeur d'un attribut - Specialisation Vector 3D
template<> inline QVector3D CXmlElement::getAttributeValue<QVector3D>(
        const QString& attributeName,
        const QVector3D& defaultValue) const
{
    if (m_Attributes.find(attributeName) != m_Attributes.end())
    {
        QVector3D attribute = defaultValue;
        QString sAttribute = m_Attributes[attributeName];

        QStringList strList = sAttribute.split(";");

        if (strList.size() == 3)
        {
            attribute.setX(strList[0].toFloat());
            attribute.setY(strList[1].toFloat());
            attribute.setZ(strList[2].toFloat());
        }
        else
        {
            qDebug() << "Erreur getAttributeValue<QVector3D>() : " << attributeName;
        }

        if (m_bDebugMode)
        {
            qDebug() << getPath() << "." << attributeName << " = " << attribute;
        }

        return attribute;
    }

    //qDebug() << "Warning, Attribute not found: " << attributeName;

    return defaultValue;
}

//! Retourne la valeur d'un attribut - Specialisation Vector 4D
template<> inline QVector4D CXmlElement::getAttributeValue<QVector4D>(
        const QString& attributeName,
        const QVector4D& defaultValue) const
{
    if (m_Attributes.find(attributeName) != m_Attributes.end())
    {
        QVector4D attribute = defaultValue;
        QString sAttribute = m_Attributes[attributeName];

        QStringList strList = sAttribute.split(";");

        if (strList.size() == 4)
        {
            attribute.setX(strList[0].toFloat());
            attribute.setY(strList[1].toFloat());
            attribute.setZ(strList[2].toFloat());
            attribute.setW(strList[3].toFloat());
        }
        else if (strList.size() == 3)
        {
            attribute.setX(strList[0].toFloat());
            attribute.setY(strList[1].toFloat());
            attribute.setZ(strList[2].toFloat());
        }
        else
        {
            qDebug() << "Erreur getAttributeValue<QVector4D>() : " << attributeName;
        }

        if (m_bDebugMode)
        {
            qDebug() << getPath() << "." << attributeName << " = " << attribute;
        }

        return attribute;
    }

    //qDebug() << "Warning, Attribute not found: " << attributeName;

    return defaultValue;
}

template<class T> inline void CXmlElement::addAttribute(const QString& name, const T& value)
{
    m_Attributes[name] = QVariant(value).toString();
}

#endif // XMLELEMENT_H
