#include "AWashable.h"

AWashable::AWashable()
: m_bIsDirty(true) {}

void AWashable::setDirty()
{
    m_bIsDirty = true;
}

void AWashable::wash()
{
    m_bIsDirty = false;
}

bool AWashable::isDirty() const
{
    return m_bIsDirty;
}
