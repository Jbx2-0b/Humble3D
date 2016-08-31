#ifndef CANIMATIONHELPER_H
#define CANIMATIONHELPER_H

// Qt
#include <QQuaternion>
#include <QVector3D>

// Foundations
#include "CVectorKey.h"
#include "CQuaternionKey.h"
#include "CMeshKey.h"


namespace CAnimationHelper
{

//-------------------------------------------------------------------------------------------------
// Classe utilitaires permettant de réaliser des interpolations
// Les quaternions sont interpolés via la fonctions nlerp()
// Les autres types par une simple interpolation linéaire
//-------------------------------------------------------------------------------------------------

template <typename T>
struct Interpolator
{
    void operator () (T& out, const T& a, const T& b, real d) const
    {
        out = a + (b - a) * d;
    }
};

//-------------------------------------------------------------------------------------------------
template <>
struct Interpolator<QQuaternion>
{
    void operator () (QQuaternion& out, const QQuaternion& a, const QQuaternion& b, real d) const
    {
        //interpolate(out, a, b, d);
        out = QQuaternion::nlerp(a, b, d);
    }
};

//-------------------------------------------------------------------------------------------------
template <>
struct Interpolator	<unsigned int>
{
    void operator () (unsigned int& out, unsigned int a, unsigned int b, real d) const
    {
        out = d > 0.5f ? b : a;
    }
};

//-------------------------------------------------------------------------------------------------
template <>
struct Interpolator<CVectorKey>
{
    void operator () (QVector3D& out, const CVectorKey& a, const CVectorKey& b, real d) const
    {
        Interpolator<QVector3D> ipl;
        ipl(out, a.getValue(), b.getValue(), d);
    }
};

//-------------------------------------------------------------------------------------------------
template <>
struct Interpolator <CQuaternionKey>
{
    void operator () (QQuaternion& out, const CQuaternionKey& a, const CQuaternionKey& b, real d) const
    {
        Interpolator<QQuaternion> ipl;
        ipl(out, a.getValue(), b.getValue(), d);
    }
};

//-------------------------------------------------------------------------------------------------
template <>
struct Interpolator <CMeshKey>
{
    void operator () (unsigned int& out, const CMeshKey& a,	const CMeshKey& b, real d) const
    {
        Interpolator<unsigned int> ipl;
        ipl(out, a.getValue(), b.getValue(), d);
    }
};



};

#endif // CANIMATIONHELPER_H
