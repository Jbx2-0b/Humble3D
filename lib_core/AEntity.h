#ifndef CENTITY_H
#define CENTITY_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CIDGenerator.h"
#include "CNameGenerator.h"
#include "CLogManager.h"
#include "ALockable.h"
#include "AWashable.h"

// Qt
#include <QString>
#include <QList>
#include <QDebug>


//-----------------------------------------------------------------------------------------
// \class AEntity
// \brief Méta-classe permettant de manipuler des entitées - Sert de meta-objet pour les éléments 3D
//-----------------------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT AEntity : public ALockable
{
public:

    //! Constructeur
    AEntity();

    //! Constructeur
    AEntity(const QString& name);

    //! Destructeur
    virtual ~AEntity();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit le nom
    void setName(const QString& name);

    //! Définit si les notifications sont activées
    void setNotificationsEnabled(bool bEnabled) { m_bNotificationsEnabled = bEnabled; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const = 0;

    //! Retourne le nom
    const QString& getName() const { return m_Name; }

    //! Retourne l'ID
    int getID() const { return m_iID; }

    //! Retourne vrai si les notifications sont activées
    bool isNotificationsEnabled() const { return m_bNotificationsEnabled; }

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[Entity \n";
        result += QString("Name: %1\n").arg(m_Name);
        result += QString("ID: %1\n").arg(m_iID);
        result += QString("Notifications: %1\n").arg(m_bNotificationsEnabled);
        result += "]\n";
        return result;
    }

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IEntityListener
    {
        friend class AEntity;

    protected:

        virtual void onUpdate(AEntity* pEntity) = 0;
        virtual void onDelete(AEntity* pEntity) = 0;
    };

    //! Enregistre un listener
    void registerListener(IEntityListener* pListener)
    {
        m_EntityListeners.insert(pListener);
    }

    //! Libére un listener
    void unregisterListener(IEntityListener* pListener)
    {
        m_EntityListeners.remove(pListener);
    }

protected:

    //! Notifie d'une mise à jour
    virtual void notifyUpdate()
    {
        if (m_bNotificationsEnabled)
        {
            for (IEntityListener* pListener : m_EntityListeners)
            {
                pListener->onUpdate(this);
            }
        }
    }

    //! Notifie d'une suppression
    virtual void notifyDelete()
    {
        if (m_bNotificationsEnabled)
        {
            for (IEntityListener* pListener : m_EntityListeners)
            {
                pListener->onDelete(this);
            }
        }
    }

    //! Liste des listeners
    QSet<IEntityListener*> m_EntityListeners;

    //! Nom
    QString m_Name;

    //! Vrai si les notifications sont activées
    bool m_bNotificationsEnabled;

private:

    //! ID unique
    int m_iID;

    //! Generateur d'ID
    static CIDGenerator s_IDGenerator;

    //! Generateur de noms
    static CNameGenerator s_NameGenerator;
};




#endif //CENTITY_H 
