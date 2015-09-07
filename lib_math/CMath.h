#ifndef MATH_H
#define MATH_H

// Lib
#include "lib_math_global.h"

// STL
#include <cmath>
#include <limits>

// Qt
#include <QtGlobal>
#include <QtCore/qmath.h>
#include <QTime>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV_64BITS
#else
#define ENV_32BITS
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV_64BITS
#else
#define ENV_32BITS
#endif
#endif

#ifdef ENV_64BITS
    typedef double real;
#else // ENV_32BITS
    typedef float real;
#endif


class LIB_MATHSHARED_EXPORT Math
{
public:

    static const real Pi;
    static const real TwoPi;
    static const real HalfPi;
    static const real Sqrt2;

    static const QMatrix4x4 ZeroMatrix;

    //! Initialise une graine pour les nombres al�atoires
    static void initRand();
	
    //! Retourne un nombre al�atoire entre 0. et 1.
    static real randNorm();

    //! Retourne un nombre al�atoire d�cimal entre min et max
    static real randDouble(real min, real max);

    //! Retourne un nombre al�atoire entier entre min et max
    static int randInt(int min, int max);

    //! Retourne un vecteur al�atoire entre min et max
    static QVector2D randVector(const QVector2D& vectMin, const QVector2D& vectMax);

    //! Retourne un vecteur al�atoire entre [-Pi; Pi]
    static real randRadianAngle();

    //! Retourne un vecteur al�atoire entre [0; 360]
    static real randDegreeAngle();

    //! Retourne un nombre al�atoire entre 0. et 1.
    static real rand(const QVector2D& co);

    //! Limite dValue entre [dMin; dMax[
    static real clipDouble(real dValue, real dMin, real dMax);

    //! Limite dAngle entre [0; 360[
    static real clipAngleDegree(real dAngle);

    //! Diff�rence entre deux angles
    static real angleDifferenceDegree(real dAngle1, real dAngle2);

    //! retourne un angle compris dans [ 0;2PI [
    static real modulo2PI(real radAngle);

    //! limite n dans [-Pi; Pi]
    static real rangePIMinusPI(real n);

    //! transforme un angle degre en radians
    static real degToRad(const real angleDeg);

    //! transforme un angle radians en degree
    static real radToDeg(const real angleRad);

    //! transforme un angle radians en milli�mes sens horraire
    static real radToMil(const real angleRad);

    //! transforme un angle milli�mes en radians sens horraire
    static real milToRad(const real angleMil);

    //! Radians to Gradians
    static real radToGrad(real dRad);

    //! Gradians to radians
    static real gradToRad(real dGrad);

    //! Arrondi
    static int round(real dValue);

    //! Borne un nombre entre [0; pMax]
    static int clamp(int pX, int pMax);

    //! Interpolation lin�aire entre deux �l�ments
    static real interpolate(real dStart, real dEnd, real dValue);

    //! Return power of 2
    static real squared(real v);

    //! Retourne la partie fractionnelle de x
    static real fract(real x);

    //! Determine la distance entre deux points 2D
    static real distance2D(const QPointF &A, const QPointF &B);

    //! Retourne la puissance de deux superieure
    static real nextPowerOf2(real base);

    //! permet de savoir si le segment [AB] coupe le segment [CD]
    static bool intersect(const QPoint &A, const QPoint &B, const QPoint &C, const QPoint &D);

    //! retourne le point d'intersection entre le segment [AB] et le segment [CD]. Si aucun retourne l'origine.
    static QPoint intersectPoint(const QPoint &A, const QPoint &B, const QPoint &C, const QPoint &D);

    //! verifie l'egalite entre deux doubles a un epsilon pret
    static bool isZero(real value, real epsilon = 0.000001);

    //! verifie l'egalite entre deux doubles a un epsilon pret
    static bool isEqual(real a, real b, real epsilon = 0.000001);

    //! Teste si un nombre est fini
    static bool isFinite(real x);

    //! Transforme une position en pixel dans l'image en d�placement en degr�
    static QPointF computePixToDeg(QSize videoSize, real dCameraFOV, QPointF screen2DPoint, bool bNormalized = false);

    //! Transforme un d�placement en d�gr� en position en pixel dans l'image - Attention le r�sultat n'est pas normalis�
    static QPoint computeDegToPix(QSize videoSize, real dCameraFOV, QPointF angles);

    //! Divise v1 par v2 et retourne le r�sultat
    static QVector3D divide(const QVector3D& v1, const QVector3D& v2);

    //! Recup�re les composants minimum de deux vecteurs
    static QVector3D makeFloor(const QVector3D& v1, const QVector3D& v2);

    //! Recup�re les composants maximum de deux vecteurs
    static QVector3D makeCeil(const QVector3D& v1, const QVector3D& v2);

    //! returns a value equal to the nearest integer that is less than or equal to x.
    //static QVector2D floor(const QVector2D& v);

    //! Retourne la partie fractionnelle de x
    //static QVector2D fract(const QVector2D& v);

    //! Interpolation lin�aire : relie deux points par un segment, x E [0; 1]
    static double linearInterpolation(double a, double b, double x);

    //! Interpolation lin�aire : relie deux points par un segment, x E [0; 1], y E [0; 1]
    static double linearInterpolation2D(double a, double b, double c, double d, double x, double y);

    //! Interpolation cosinus : bas�e sur la fonction (1-cos(Pi.x)) / 2., x E [0; 1]
    static double cosInterpolation(double a, double b, double x);

    //! Interpolation cosinus : bas�e sur la fonction (1-cos(Pi.x)) / 2., x E [0; 1], y E [0; 1]
    static double cosInterpolation2D(double a, double b, double c, double d, double x, double y);

    //! Interpolation cubique bas�e sur le polyn�me f(x)=ax3+bx2+cx+d
    static double cubicInterpolation(double y0, double y1, double y2, double y3, double x);

    //! Interpolation cubique bas�e sur le polyn�me f(x)=ax3+bx2+cx+d
    static double cubicInterpolation2D(double y00, double y01, double y02, double y03,
        double y10, double y11, double y12, double y13, double y20, double y21, double y22,
        double y23, double y30, double y31, double y32, double y33, double x, double y);

    //! saturate - returns smallest integer not less than a scalar
    static real saturate(real x);

    //! interpolate smoothly between two input values based on a third
    static real smoothstep(real a, real b, real x);

    //! Rotated vector around axis
    static QVector3D rotateAroundAxis(const QVector3D& vect, const QVector3D& axis, double theta);

    //! Return value sign
    template <typename T> static int sign(T val)
    {
        return (T(0) < val) - (val < T(0)) >= 0 ? 1 : -1;
    }

    //! Angle between two vector
    static real computeRadAngle(const QVector3D& v1, const QVector3D& v2);

    //! Angle between two vector
    static real computeRadAngle(const QVector2D& vOrigin, const QVector2D& vDirection);

    static real absDotProduct(const QVector3D& vec1, const QVector3D& vec2);

    //! Retourne une matrice d�crivant une projection infinie
    static QMatrix4x4 infinitePerspective(real dFOVY, real dAspect, real dZNear);

    //! R�alise l'interpolation entre deux quaternions
    void interpolate(QQuaternion& pOut, const QQuaternion& pStart, const QQuaternion& pEnd, real pFactor);


private:

    static bool bRandInitialized;
};


#endif //MATH_H

