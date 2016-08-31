#include "CMath.h"


bool Math::bRandInitialized = false;

const QMatrix4x4 Math::ZeroMatrix = QMatrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

#ifdef ENV_64BITS
const real Math::Pi		= 3.141592653589793238462643383279;
const real Math::TwoPi	= Pi * 2.;
const real Math::HalfPi	= Pi * 0.5;
const real Math::Sqrt2	= 1.41421356237309504880;
#else // ENV_32BITS
const real Math::Pi		= 3.1415926538f;
const real Math::TwoPi	= Pi * 2.f;
const real Math::HalfPi	= Pi * 0.5f;
const real Math::Sqrt2	= 1.4142135624f;
#endif


void Math::initRand()
{
    if (!bRandInitialized)
    {
        qsrand(QTime::currentTime().elapsed());
        bRandInitialized = true;
    }
}

real Math::randNorm()
{
    return ((real) qrand()) / RAND_MAX;
}

real Math::randDouble(real min, real max)
{
    initRand();
    return ((real) qrand() / RAND_MAX * (max - min)) + min;
}

int Math::randInt(int min, int max)
{
    initRand();
    return (int)(((real)qrand() / RAND_MAX * (max - min)) + min);
}

QVector2D Math::randVector(const QVector2D& vectMin, const QVector2D& vectMax)
{
    initRand();
    return QVector2D(randDouble(vectMin.x(), vectMax.x()), randDouble(vectMin.y(), vectMax.y()));
}

real Math::randRadianAngle()
{
    initRand();
    return randNorm() * 2. * Math::Pi - Math::Pi;
}

real Math::randDegreeAngle()
{
    initRand();
    return randDouble(0., 360.);
}

real Math::rand(const QVector2D& co)
{
    return Math::fract(sin(QVector2D::dotProduct(co, QVector2D(12.9898f, 78.233f))) * 43758.5453f);
}

real Math::clipDouble(real dValue, real dMin, real dMax)
{
    if (dValue < dMin) dValue = dMin;
    if (dValue > dMax) dValue = dMax;

    return dValue;
}

real Math::clipAngleDegree(real dAngle)
{
    while (dAngle <   0.0) dAngle += 360.0;
    while (dAngle > 360.0) dAngle -= 360.0;

    return dAngle;
}

real Math::angleDifferenceDegree(real dAngle1, real dAngle2)
{
    real a = dAngle1 - dAngle2;
    a += (a > 180) ? -360 : (a < -180) ? 360 : 0;
    return a;
}

real Math::modulo2PI(real radAngle)
{
    while(radAngle >= 2*Pi)
    {
        radAngle -= 2*Pi;
    }
    while(radAngle < 0)
    {
        radAngle += 2*Pi;
    }
    return radAngle;
}

real Math::rangePIMinusPI(real n)
{
    if (n > Math::Pi)
    {
        n -= 2*Math::Pi;
    }
    else if (n < -Math::Pi)
    {
        n += 2*Math::Pi;
    }

    return n;
}

real Math::degToRad(const real angleDeg)
{
    return (angleDeg / 360.) * (2. * Pi);
}

real Math::radToDeg(const real angleRad)
{
    return (angleRad / (2. * Pi)) * 360.;
}

real Math::radToMil(const real angleRad)
{
    return (modulo2PI(angleRad) / (2. * Pi)) * 6400;
}

real Math::milToRad(const real angleMil)
{
    return (angleMil/6400.) * (2. * Pi);
}

real Math::radToGrad(real dRad)
{
    return (dRad * 180.0) / Pi;
}

real Math::gradToRad(real dGrad)
{
    return (dGrad / 180.0) * Pi;
}

int Math::round(real dValue)
{
    if ((dValue - (int) dValue) > 0.5)
        return (int) dValue + 1;
    return (int) dValue;
}

int Math::clamp(int pX, int pMax)
{
    if (pX > pMax)
    {
        return pMax;
    }
    else if (pX < 0)
    {
        return 0;
    }

    return pX;
}

real Math::interpolate(real dStart, real dEnd, real dValue)
{
    return dStart * (1. - dValue) + dEnd * dValue;
}

real Math::squared(real v)
{
    return v * v;
}

real Math::fract(real x)
{
    return x - floor(x);
}

real Math::distance2D(const QPointF &A, const QPointF &B)
{
    return sqrt(pow(B.x() - A.x(), 2) + pow(B.y() - A.y(), 2));
}

real Math::nextPowerOf2(real base)
{
    int pOf2 = 2;
    while (base > pOf2)
    {
        pOf2 = qPow(pOf2, 2);
    }
    return pOf2;
}

bool Math::intersect(const QPoint &A, const QPoint &B, const QPoint &C, const QPoint &D)
{
    QVector2D I(B - A);
    QVector2D J(D - C);

    if (I.x() * J.y() - I.y() * J.x() != 0) // intersection des droites
    {
        real m = -(-I.x()*A.y()+I.x()*C.y()+I.y()*A.x()-I.y()*C.x()) / (I.x()*J.y()-I.y()*J.x());
        real k = -(A.x()*J.y()-C.x()*J.y()-J.x()*A.y()+J.x()*C.y()) / (I.x()*J.y()-I.y()*J.x());

        if (m < 1.0 && m > 0 && k < 1.0 && k > 0) // intersection des segments
        {
            return true;
        }
    }

    return false;
}

QPoint Math::intersectPoint(const QPoint &A, const QPoint &B, const QPoint &C, const QPoint &D)
{
    QVector2D I(B - A);
    QVector2D J(D - C);

    if (I.x()*J.y()-I.y()*J.x() != 0) // intersection des droites
    {
        real m = -(-I.x()*A.y()+I.x()*C.y()+I.y()*A.x()-I.y()*C.x()) / (I.x()*J.y()-I.y()*J.x());
        real k = -(A.x()*J.y()-C.x()*J.y()-J.x()*A.y()+J.x()*C.y()) / (I.x()*J.y()-I.y()*J.x());

        if (m < 1.0 && m > 0 && k < 1.0 && k > 0) // intersection des segments
        {
            return QPoint(C.x() + m*J.x(), C.y() + m*J.y());
        }
    }

    return QPoint();
}

bool Math::isZero(real value, real epsilon)
{
    return fabs(value) <= epsilon;
}

bool Math::isEqual(real a, real b, real epsilon)
{
    return fabs(a - b) <= epsilon;
}

bool Math::isFinite(real x)
{
    const real INF = std::numeric_limits<real>::infinity();
    return -INF < x && x < INF;
}

QPointF Math::computePixToDeg(QSize videoSize, real dCameraFOV, QPointF screen2DPoint, bool bNormalized)
{
    real dCameraFOVW = dCameraFOV;
    real dCameraFOVH = (dCameraFOV / (real) videoSize.width()) * (real) videoSize.height();

    real dPixX;
    real dPixY;

    // Dans le cas ou les coordonnées de points ont était normalisées on les dénormalise avant d'effectuer le calcul
    if (bNormalized)
    {
        dPixX = (real) (screen2DPoint.x() * videoSize.width()  - videoSize.width() / 2);
        dPixY = (real) (screen2DPoint.y() * videoSize.height() - videoSize.height() / 2);
    }
    else
    {
        dPixX = (real) (screen2DPoint.x() - videoSize.width() / 2);
        dPixY = (real) (screen2DPoint.y() - videoSize.height() / 2);
    }


    real dLW = (videoSize.width() / 2.) / tan(degToRad(dCameraFOVW / 2.0));
    real dLH = (videoSize.height() / 2.) / tan(degToRad(dCameraFOVH / 2.0));

    real dDegX = radToDeg(atan(dPixX / dLW));
    real dDegY = radToDeg(atan(dPixY / dLH));

    // L'axe Y est inversé entre le répére écran et le repére angulaire
    return QPointF(dDegX, -dDegY);
}

QPoint Math::computeDegToPix(QSize videoSize, real dCameraFOV, QPointF angles)
{
    real dCameraFOVW = dCameraFOV;
    real dCameraFOVH = (dCameraFOV / (real) videoSize.width()) * (real) videoSize.height();

    real dLW = (videoSize.width() / 2.) / tan(degToRad(dCameraFOVW / 2.0));
    real dLH = (videoSize.height() / 2.) / tan(degToRad(dCameraFOVH / 2.0));

    int dPixX = (int) (dLW * tan(degToRad(angles.x())));
    int dPixY = (int) (dLH * tan(degToRad(angles.y())));

    // L'axe Y est inversé entre le répére écran et le repére angulaire
    return QPoint(videoSize.width() / 2 + dPixX, videoSize.height() / 2 - dPixY);
}

QVector3D Math::divide(const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

QVector3D Math::makeFloor(const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D(qMin(v1.x(), v2.x()), qMin(v1.y(), v2.y()), qMin(v1.z(), v2.z()));
}

QVector3D Math::makeCeil(const QVector3D& v1, const QVector3D& v2)
{
    return QVector3D(qMax(v1.x(), v2.x()), qMax(v1.y(), v2.y()), qMax(v1.z(), v2.z()));
}

//QVector2D Math::floor(const QVector2D& v)
//{
//    return QVector2D(floor(v.x()), floor(v.y()));
//}

//QVector2D Math::fract(const QVector2D& v)
//{
//    return v - floor(v);
//}

double Math::linearInterpolation(double a, double b, double x)
{
    return a * (1 - x) + b * x;
}

double Math::linearInterpolation2D(double a, double b, double c, double d, double x, double y)
{
    double dI1 = linearInterpolation(a, b, x);
    double dI2 = linearInterpolation(c, d, x);
    return linearInterpolation(dI1, dI2, y);
}

double Math::cosInterpolation(double a, double b, double x)
{
    double k = (1 - cos(x * Math::Pi)) / 2;
    return linearInterpolation(a, b, k);
}

double Math::cosInterpolation2D(double a, double b, double c, double d, double x, double y)
{
    double x1 = cosInterpolation(a, b, x);
    double x2 = cosInterpolation(c, d, x);
    return linearInterpolation(x1, x2, y);
}

double Math::cubicInterpolation(double y0, double y1, double y2, double y3, double x)
{
    double a = y3 - y2 - y0 + y1;
    double b = y0 - y1 - a;
    double c = y2 - y0;
    double d = y1;

    return a *x * x * x + b * x * x + c * x + d;
}

double Math::cubicInterpolation2D(double y00, double y01, double y02, double y03,
                                  double y10, double y11, double y12, double y13, double y20, double y21, double y22,
                                  double y23, double y30, double y31, double y32, double y33, double x, double y)
{
    double v0 = cubicInterpolation(y00,y01,y02,y03,x);
    double v1 = cubicInterpolation(y10,y11,y12,y13,x);
    double v2 = cubicInterpolation(y20,y21,y22,y23,x);
    double v3 = cubicInterpolation(y30,y31,y32,y33,x);

    return cubicInterpolation(v0,v1,v2,v3,y);
}

real Math::saturate(real x)
{
    return qMax((real)0., qMin((real)1., x));
}

real Math::smoothstep(real a, real b, real x)
{
    real t = saturate((x - a)/(b - a));
    return t*t*(3.0 - (2.0*t));
}

QVector3D Math::rotateAroundAxis(const QVector3D& vect, const QVector3D& axis, double theta)
{
    double u = axis.x();
    double v = axis.y();
    double w = axis.z();

    double cosTheta = qCos(theta);
    double sinTheta = qSin(theta);

    double ms = axis.lengthSquared();
    double m = qSqrt(ms);

    return QVector3D(
                ((u * (u * vect.x() + v * vect.y() + w * vect.z())) +
                 (((vect.x() * (v * v + w * w)) - (u * (v * vect.y() + w * vect.z()))) * cosTheta) +
                 (m * ((-w * vect.y()) + (v * vect.z())) * sinTheta)) / ms,

                ((v * (u * vect.x() + v * vect.y() + w * vect.z())) +
                 (((vect.y() * (u * u + w * w)) - (v * (u * vect.x() + w * vect.z()))) * cosTheta) +
                 (m * ((w * vect.x()) - (u * vect.z())) * sinTheta)) / ms,

                ((w * (u * vect.x() + v * vect.y() + w * vect.z())) +
                 (((vect.z() * (u * u + v * v)) - (w * (u * vect.x() + v * vect.y()))) * cosTheta) +
                 (m * (-(v * vect.x()) + (u * vect.y())) * sinTheta)) / ms);
}

real Math::computeRadAngle(const QVector3D& v1, const QVector3D& v2)
{
    int iSign = sign(QVector3D::crossProduct(v1, v2).z());

    return iSign * acos(QVector3D::dotProduct(v1.normalized(), v2.normalized()));
}

real Math::computeRadAngle(const QVector2D& vOrigin, const QVector2D& vDirection)
{
    QVector3D vOrigin3D(vOrigin.x(), vOrigin.y(), 0.0);
    QVector3D vDirection3D(vDirection.x(), vDirection.y(), 0.0);

    int iSign = sign(QVector3D::crossProduct(vOrigin3D, vDirection3D).z());
    return iSign * acos(QVector3D::dotProduct(vOrigin3D.normalized(), vDirection3D.normalized()));
}

real Math::absDotProduct(const QVector3D& vec1, const QVector3D& vec2)
{
    return qAbs(vec1.x() * vec2.x()) + qAbs(vec1.y() * vec2.y()) + qAbs(vec1.z() * vec2.z());
}

QMatrix4x4 Math::infinitePerspective(real dFOVY, real dAspect, real dZNear)
{
    const real range = tan(dFOVY / real(2)) * dZNear;

    real left = -range * dAspect;
    real right = range * dAspect;
    real bottom = -range;
    real top = range;

    QMatrix4x4 result = ZeroMatrix;
    result(0, 0) = (real(2) * dZNear) / (right - left);
    result(1, 1) = (real(2) * dZNear) / (top - bottom);
    result(2, 2) = - real(1);
    result(2, 3) = - real(1);
    result(3, 2) = - real(2) * dZNear;

    return result;
}

void Math::interpolate(QQuaternion& pOut, const QQuaternion& pStart, const QQuaternion& pEnd, real pFactor)
{
    // calc cosine theta
    real cosom = pStart.x() * pEnd.x() + pStart.y() * pEnd.y() + pStart.z() * pEnd.z() + pStart.scalar() * pEnd.scalar();

    // adjust signs (if necessary)
    QQuaternion end = pEnd;
    if (cosom < 0.0)
    {
        cosom = -cosom;
        end.setX(-end.x());   // Reverse all signs
        end.setY(-end.y());
        end.setZ(-end.z());
        end.setScalar(-end.scalar());
    }

    // Calculate coefficients
    real sclp, sclq;
    if ((1.0 - cosom) > 0.0001) // 0.0001 -> some epsillon
    {
        // Standard case (slerp)
        real omega, sinom;
        omega = acos(cosom); // extract theta from dot product's cos theta
        sinom = sin(omega);
        sclp  = sin((1.0 - pFactor) * omega) / sinom;
        sclq  = sin(pFactor * omega) / sinom;
    }
    else
    {
        // Very close, do linear interp (because it's faster)
        sclp = 1.0 - pFactor;
        sclq = pFactor;
    }

    pOut.setX(sclp * pStart.x() + sclq * end.x());
    pOut.setY(sclp * pStart.y() + sclq * end.y());
    pOut.setZ(sclp * pStart.z() + sclq * end.z());
    pOut.setScalar(sclp * pStart.scalar() + sclq * end.scalar());
}
