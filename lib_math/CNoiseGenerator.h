#ifndef CNOISEGENERATOR_H
#define CNOISEGENERATOR_H

// Lib
#include "lib_math_global.h"
#include "CMath.h"

// Qt
#include <QVector>


enum EnumTypeInterpolation
{
    eInterpolationLinear,
    eInterpolationCosinus,
    eInterpolationCubic
};

class LIB_MATHSHARED_EXPORT CNoiseGenerator1D
{
public:

    CNoiseGenerator1D(int iSize, int iStep, int iOctave);

    double getNoise(int i) const { return m_Values[i]; }

    double fNoise(double x) const;

    double fCoherentNoise(double x, double persistance) const;

private:
    int m_iSize;
    int m_iStep;
    int m_iOctave;

    QVector<double> m_Values;
};

class LIB_MATHSHARED_EXPORT CNoiseGenerator2D
{
public:
    CNoiseGenerator2D(int iWidth, int iHeight, int iStep, int iOctave);

    double getNoise(int i, int j) const { return m_Values[i * m_iMaxWidth + j]; }

    double fNoise(double x, double y) const;

    double fCoherentNoise(double x, double y, double persistance) const;

    QImage toImage(double dPersistance) const;

    QImage toImage(double dPersistance, const QList<QColor>& colors, const QList<double>& thresholds) const;

private:
    int m_iWidth;
    int m_iMaxWidth;
    int m_iHeight;
    int m_iStep;
    int m_iOctave;

    QVector<double> m_Values;

    QColor threshold(double dValue, const QList<QColor>& colors, const QList<double>& thresholds) const;
};

#endif // CNOISEGENERATOR_H
