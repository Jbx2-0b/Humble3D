#include "CNoiseGenerator.h"
#include "CMath.h"

#include <QDebug>
#include <QImage>
#include <QColor>

//-----------------------------------------------------------------------------------------
CNoiseGenerator1D::CNoiseGenerator1D(int iSize, int iStep, int iOctave)
    : m_iSize(iSize + 1)
    , m_iStep(iStep)
    , m_iOctave(iOctave)
{
    m_Values.resize((int) ceil(m_iSize * pow(2., iOctave  - 1)  / iStep));
    Math::initRand();

    for (int i = 0; i < m_Values.size(); ++i)
    {
        m_Values[i] = Math::randNorm();
    }
}

//-----------------------------------------------------------------------------------------
double CNoiseGenerator1D::fNoise(double x) const
{
    int i = (int) (x / m_iStep);
    return Math::cosInterpolation(getNoise(i), getNoise(i + 1), fmod(x / m_iStep, 1));
}

//-----------------------------------------------------------------------------------------
double CNoiseGenerator1D::fCoherentNoise(double x, double persistance) const
{
    double sum = 0;
    double p = 1;
    int f = 1;

    for(int i = 0 ; i < m_iOctave ; ++i)
    {
        sum += p * fNoise(x * f);
        p *= persistance;
        f *= 2;
    }
    return sum * (1 - persistance) / (1 - p);
}

//-----------------------------------------------------------------------------------------
CNoiseGenerator2D::CNoiseGenerator2D(int iWidth, int iHeight, int iStep, int iOctave)
    : m_iWidth(iWidth + 1)
    , m_iHeight(iHeight + 1)
    , m_iStep(iStep)
    , m_iOctave(iOctave)
{
    m_iMaxWidth = (int) ceil(m_iWidth * pow(2., iOctave  - 1)  / iStep);
    int iMaxHeight = (int) ceil(m_iHeight * pow(2., iOctave  - 1)  / iStep);

    m_Values.resize(m_iMaxWidth * iMaxHeight);
    Math::initRand();

    for (int i = 0; i < m_Values.size(); ++i)
    {
        m_Values[i] = Math::randNorm();
    }
}

//-----------------------------------------------------------------------------------------
double CNoiseGenerator2D::fNoise(double x, double y) const
{
    int i = (int) (x / m_iStep);
    int j = (int) (y / m_iStep);
    return Math::cosInterpolation2D(getNoise(i, j), getNoise(i + 1, j), getNoise(i, j + 1), getNoise(i + 1, j + 1), fmod(x / m_iStep, 1), fmod(y / m_iStep, 1));
}

//-----------------------------------------------------------------------------------------
double CNoiseGenerator2D::fCoherentNoise(double x, double y, double persistance) const
{
    double sum = 0;
    double p = 1;
    int f = 1;

    for(int i = 0 ; i < m_iOctave ; ++i)
    {
        sum += p * fNoise(x * f, y * f);
        p *= persistance;
        f *= 2;
    }

    return sum * (1 - persistance) / (1 - p);
}

//-----------------------------------------------------------------------------------------
QImage CNoiseGenerator2D::toImage(double dPersistance) const
{
    QImage img(m_iWidth - 1, m_iHeight - 1, QImage::Format_RGB32);

    for (int i = 0; i < m_iWidth - 1; i++)
    {
        for (int j = 0; j < m_iHeight - 1; j++)
        {
            double dValue = fCoherentNoise(i, j, dPersistance);
            QColor color(dValue * 255., dValue * 255., dValue * 255.);
            img.setPixel(i, j, color.rgb());
        }
    }

    return img;
}

//-----------------------------------------------------------------------------------------
QColor CNoiseGenerator2D::threshold(double dValue, const QList<QColor>& colors, const QList<double>& thresholds) const
{
    QColor color;

    if (dValue <= thresholds[0])
    {
        color = colors[0];
    }
    else
    {
        bool bInter = false;
        for (int  i = 1; i < thresholds.size(); ++i)
        {
            if (dValue < thresholds[i])
            {
                double f = (dValue - thresholds[i-1]) / (thresholds[i] - thresholds[i-1]);
                color.setRedF	(colors[i-1].redF()		* (1 - f) + colors[i].redF() * f);
                color.setGreenF	(colors[i-1].greenF()	* (1 - f) + colors[i].greenF() * f);
                color.setBlueF	(colors[i-1].blueF()	* (1 - f) + colors[i].blueF() * f);
                bInter = true;
                break;
            }
        }

        if (!bInter)
        {
            color = colors[colors.size() - 1];
        }
    }

    return color;
}
//-----------------------------------------------------------------------------------------
QImage CNoiseGenerator2D::toImage(double dPersistance, const QList<QColor>& colors, const QList<double>& thresholds) const
{
    QImage img(m_iWidth - 1, m_iHeight - 1, QImage::Format_RGB32);

    for (int i = 0; i < m_iWidth - 1; i++)
    {
        for (int j = 0; j < m_iHeight - 1; j++)
        {
            double dValue = fCoherentNoise(i, j, dPersistance);
            QColor color = threshold(dValue, colors, thresholds);
            img.setPixel(i, j, color.rgb());
        }
    }

    return img;
}
