#include "AEntity.h"

// Définition des membres statiques
CIDGenerator AEntity::s_IDGenerator;

CNameGenerator AEntity::s_NameGenerator;


//-----------------------------------------------------------------------------------------
AEntity::AEntity()
    : m_bNotificationsEnabled(true)
    , m_iID(s_IDGenerator.generateID(this))
{
    m_Name = s_NameGenerator.generateName("Entity");

    pLog->addMessage(eDEBUGMEMORY, "Creation : " + m_Name);
}

//-----------------------------------------------------------------------------------------
AEntity::AEntity(const QString& name)
    : m_bNotificationsEnabled(true)
    , m_iID(s_IDGenerator.generateID(this))
{
    m_Name = s_NameGenerator.generateName(name);

    pLog->addMessage(eDEBUGMEMORY, "Creation : " + m_Name);
}

//-----------------------------------------------------------------------------------------
AEntity::~AEntity()
{
    pLog->addMessage(eDEBUGMEMORY, "Suppression : " + m_Name);
    s_IDGenerator.unregisterID(m_iID);
    notifyDelete();
}

//-----------------------------------------------------------------------------------------
void AEntity::setName(const QString& name)
{
    if (name != m_Name)
    {
        m_Name = s_NameGenerator.generateName(name);
        notifyUpdate();
    }
}
