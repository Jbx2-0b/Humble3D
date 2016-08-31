#ifndef CQUATERNIONKEY_H
#define CQUATERNIONKEY_H

// Qt
#include <QQuaternion>

// Foundations
#include "CMath.h"


class CQuaternionKey
{
public:
    //! Default constructor
    CQuaternionKey()
        : m_dTime(0.){}

    //! Ctor
    CQuaternionKey(real dTime, const QQuaternion& qValue)
        : m_dTime(dTime), m_qValue(qValue){}

    //! Le retourne le temps de la clef
    real getTime() const { return m_dTime; }

    //! Retourne la valeur de la clef
    QQuaternion getValue() const { return m_qValue; }

    // Comparison operators. For use with find()
    bool operator == (const CQuaternionKey& o) const
    {
        return o.m_qValue == this->m_qValue;
    }

    bool operator != (const CQuaternionKey& o) const
    {
        return o.m_qValue != this->m_qValue;
    }

    // Relational operators. For use with sort()
    bool operator < (const CQuaternionKey& o) const
    {
        return m_dTime < o.m_dTime;
    }

    bool operator > (const CQuaternionKey& o) const
    {
        return m_dTime > o.m_dTime;
    }

private:

    //! The time of this key
    real m_dTime;

    //! Value of this key
    QQuaternion m_qValue;

};

#endif // CQUATERNIONKEY_H
