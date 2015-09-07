#ifndef CVECTORKEY_H
#define CVECTORKEY_H

// Qt
#include <QVector3D>

// Foundations
#include "CMath.h"


class CVectorKey
{
public:
    //! Default constructor
    CVectorKey()
        : m_dTime(0.){}

    //! Ctor
    CVectorKey(real dTime, const QVector3D& vValue)
        : m_dTime(dTime), m_vValue(vValue){}

    //! Le retourne le temps de la clef
    real getTime() const { return m_dTime; }

    //! Retourne la valeur de la clef
    QVector3D getValue() const { return m_vValue; }

    // Comparison operators. For use with find()
    bool operator == (const CVectorKey& o) const
    {
        return o.m_vValue == this->m_vValue;
    }

    bool operator != (const CVectorKey& o) const
    {
        return o.m_vValue != this->m_vValue;
    }

    // Relational operators. For use with sort()
    bool operator < (const CVectorKey& o) const
    {
        return m_dTime < o.m_dTime;
    }

    bool operator > (const CVectorKey& o) const
    {
        return m_dTime > o.m_dTime;
    }

private:

    //! The time of this key
    real m_dTime;

    //! Value of this key
    QVector3D m_vValue;

};

#endif // CVECTORKEY_H
