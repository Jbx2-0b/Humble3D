#include "CIDGenerator.h"
#include "CLogManager.h"
#include "AEntity.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------
CIDGenerator::CIDGenerator()
: m_Mutex(QMutex::Recursive), m_iCurrentID(0)
{
}

//-----------------------------------------------------------------------------------------
CIDGenerator::~CIDGenerator()
{
    QMutexLocker locker(&m_Mutex);

    if (!m_Entities.isEmpty())
    {
        pLog->addMessage(eDEBUG, "Entities have not been released :(");
        pLog->addMessage(eDEBUG, QString("Entities count: %1").arg(m_Entities.size()));

        foreach (AEntity* pEntity, m_Entities)
        {
            pLog->addMessage(eDEBUG, QString("%1 : %2.").arg(pEntity->getTypeName()).arg(pEntity->getName()));
        }
    }
    else
    {
        pLog->addMessage(eDEBUG, "All entities have been released :)");
    }
}

//-----------------------------------------------------------------------------------------
unsigned int CIDGenerator::generateID(AEntity* pEntity)
{
    QMutexLocker locker(&m_Mutex);
    m_Entities.insert(m_iCurrentID, pEntity);
	return m_iCurrentID++;
}

//-----------------------------------------------------------------------------------------
bool CIDGenerator::isExist(unsigned int iID) const
{
    QMutexLocker locker(&m_Mutex);
    return m_Entities.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CIDGenerator::unregisterID(unsigned int iID)
{
    QMutexLocker locker(&m_Mutex);
    m_Entities.remove(iID);
}
