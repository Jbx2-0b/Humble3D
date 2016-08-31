#ifndef CMESHKEY_H
#define CMESHKEY_H

// Foundations
#include "CMath.h"


class CMeshKey
{
public:
    //! Default constructor
    CMeshKey()
        : m_dTime(0.)
        , m_uiValue(0) {}

    //! Ctor
    CMeshKey(real dTime, unsigned int iValue)
        : m_dTime(dTime)
        , m_uiValue(iValue) {}


    //! Le retourne le temps de la clef
    real getTime() const { return m_dTime; }

    //! Retourne la valeur de la clef
    unsigned int getValue() const { return m_uiValue; }

    // Comparison operators. For use with find()
    bool operator == (const CMeshKey& o) const
    {
        return o.m_uiValue == this->m_uiValue;
    }

    bool operator != (const CMeshKey& o) const
    {
        return o.m_uiValue != this->m_uiValue;
    }

    // Relational operators. For use with sort()
    bool operator < (const CMeshKey& o) const
    {
        return m_dTime < o.m_dTime;
    }

    bool operator > (const CMeshKey& o) const
    {
        return m_dTime > o.m_dTime;
    }

private:

    //! The time of this key
    real m_dTime;

    //! Value of this key
    unsigned int m_uiValue;

};

#endif // CMESHKEY_H
