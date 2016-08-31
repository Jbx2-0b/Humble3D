#ifndef CIDGENERATOR_H
#define CIDGENERATOR_H

// Qt
#include <QString>
#include <QSet>
#include <QMutex>
#include <QMutexLocker>

#include "CLogManager.h"
class AEntity;

class CIDGenerator
{
public:

    //! Constructeur
    CIDGenerator();

    //! Destructeur
    ~CIDGenerator();

    //! Genere un ID unique
    unsigned int generateID(AEntity* pEntity);

    //! Retourne true si l'ID existe deja
    bool isExist(unsigned int iID) const;

    //! Desenregistre un ID
    void unregisterID(unsigned int iID);

private:

    mutable QMutex m_Mutex;

    unsigned int m_iCurrentID;

    QHash<unsigned int, AEntity*> m_Entities;
};

#endif // CIDGENERATOR_H
