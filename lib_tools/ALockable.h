#ifndef ALOCKABLE_H
#define ALOCKABLE_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QMutexLocker>

class LIB_TOOLSSHARED_EXPORT ALockable
{
public:

    ALockable();

    //! Rend la ressource accessible
    void lock();

    //! Rend la ressource inaccessible
    void unlock();

    //! Retourne vrai si la ressource n'est pas accessible
    bool isLocked() const;


private:
    //! Mutex
    mutable QMutex m_Mutex;

    //! Vrai si est lockée
    bool m_bIsLocked;
};

#endif // ALOCKABLE_H
