#include "ALockable.h"

ALockable::ALockable()
    : m_Mutex(QMutex::Recursive)
    , m_bIsLocked(false){}

//! Rend la ressource accessible
void ALockable::lock()
{
    QMutexLocker locker(&m_Mutex);
    m_bIsLocked = true;
}

//! Rend la ressource inaccessible
void ALockable::unlock()
{
    QMutexLocker locker(&m_Mutex);
    m_bIsLocked = false;
}

//! Retourne vrai si la ressource n'est pas accessible
bool ALockable::isLocked() const
{
    QMutexLocker locker(&m_Mutex);
    return m_bIsLocked;
}
