#ifndef AWASHABLE_H
#define AWASHABLE_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QMutexLocker>

class LIB_TOOLSSHARED_EXPORT AWashable
{
public:

    AWashable();

    //! Passe l'entitée à sale = nécessite une mise à jour
    virtual void setDirty();

    //! Passe l'entitée à propre
    virtual void wash();

    //! Retourne true si le rendu est sale
    virtual bool isDirty() const;


private:

    //! Si vrai, l'entitée est sale
    bool m_bIsDirty;

};

#endif // AWASHABLE_H
