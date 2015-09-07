#ifndef CNAMEGENERATOR_H
#define CNAMEGENERATOR_H

// Tools
#include "CLogManager.h"

// Qt
#include <QString>
#include <QSet>
#include <QMutex>


class CNameGenerator
{

public:
	
    //! Constructeur
    CNameGenerator();

    //! Destructeur
    ~CNameGenerator();

	//! Genere un nom unique
    QString generateName(const QString& prefix);


private:
	
    mutable QMutex m_Mutex;

    QHash<QString, unsigned int> m_NameCount;
};

#endif // CNAMEGENERATOR_H
