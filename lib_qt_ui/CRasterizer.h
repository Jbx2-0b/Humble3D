#ifndef CRASTERIZER_H
#define CRASTERIZER_H

// Qt
#include <QVector3D>
#include <QVector>
#include <QImage>

// STL
#include <limits>

// Foundations
#include "CMath.h"
#include "CSpan.h"
#include "CEdge.h"

// Lib
#include "CQtHelper.h"

static const float DefaultZBufferValue = std::numeric_limits<float>::max();


class CRasterizer
{
public:

    //! Constructeur
    CRasterizer(int iWidth, int iHeight, const QVector4D& backgroundColor)
        : m_iWidth(iWidth)
        , m_iHeight(iHeight)
        , m_ZBuffer(iWidth * iHeight, DefaultZBufferValue)
        , m_BackgroundColor(backgroundColor)
    {
        m_Image = QImage(iWidth, iHeight, QImage::Format_ARGB32);
        m_Image.fill(Qt::black);
    }

    //! Retourne l'image rendue
    const QImage& getImage() const	{ return m_Image; }

    //! Retourne la largeur
    int getWidth() const			{ return m_iWidth; }

    //! Retourne la hauteur
    int getHeight() const			{ return m_iHeight; }

    //! Definit la taille de l'image de rendu
    virtual void setResolution(int iWidth, int iHeight)
    {
        m_iWidth = iWidth;
        m_iHeight = iHeight;
        m_Image = QImage(iWidth, iHeight, QImage::Format_ARGB32);
        m_ZBuffer = QVector<float>(iWidth * iHeight, DefaultZBufferValue);
        m_Image.fill(Qt::black);
    }

    void clear()
    {
        m_ZBuffer.fill(DefaultZBufferValue);
        m_Image.fill(Qt::black);
    }

    //! Definit la couleur de fond
    void setBackgroundColor(const QVector4D& color)
    {
        m_BackgroundColor = color;
        m_Image.fill(Qt::black);
    }

    // Helper function f as defined in the course text.  Not sure exactly what
    //  it means, on a conceptual level.
    inline int f(const real* pVert0, const real* pVert1, int x, int y) const
    {
        return (pVert0[1] - pVert1[1]) * x + (pVert1[0] - pVert0[0]) * y + pVert0[0] * pVert1[1] - pVert1[0] * pVert0[1];
    }

    // The meat of the package.  Takes three vertices in arbitrary order, with each
    //  vertex consisting of an x and y value in the first two data positions, and
    //  any arbitrary amount of extra data, and calls the passed in function on
    //  every resulting pixel, with all data values magically interpolated.
    //  The function, drawPixel, should have the form
    //   drawPixel(int x, int y, float *data)
    //  where x and y are pixel coordinates, and data is the interpolated data as
    //  defined by the vertices.
    //void raster(vertex verts[3], void (*drawPixel)(int, int, float *))
    inline void drawTriangle(const real pColor[3][4], const real pCoords[3][3]) const
    {
        // Pixel coordinates for a bounding box
        int xMin, xMax, yMin, yMax;
        // High enough that it doesn't matter
        xMin = m_iWidth + 1; yMin = m_iHeight + 1;
        // Low enough that it doesn't matter
        xMax = yMax = -1;

        // find the bounding box
        for (int i = 0; i < 3; i++)
        {
            if (pCoords[i][0] < xMin)
                xMin = pCoords[i][0];
            if (pCoords[i][1] < yMin)
                yMin = pCoords[i][1];
            if (pCoords[i][0] > xMax)
                xMax = pCoords[i][0];
            if (pCoords[i][1] > yMax)
                yMax = pCoords[i][1];
        }

        // normalizing values for the barycentric coordinates
        real fAlpha, fBeta, fGamma;

        // not sure exactly what's going on here, so read the textbook
        fAlpha	= f(pCoords[1], pCoords[2], pCoords[0][0], pCoords[0][1]);
        fBeta	= f(pCoords[2], pCoords[0], pCoords[1][0], pCoords[1][1]);
        fGamma	= f(pCoords[0], pCoords[1], pCoords[2][0], pCoords[2][1]);

        // check for zero denominators. if found, these indicate a degenerate
        // triangle which should not be drawn, so just return.
        if(fabs(fAlpha) < .0001 || fabs(fBeta) < .0001 || fabs(fGamma) < .0001)
            return;

        yMin = (int)((yMin > 0) ? yMin : 0);
        xMin = (int)((xMin > 0) ? xMin : 0);

        // go over every pixel in the bounding box
        for (int y = yMin; y < yMax && y < m_iHeight; ++y)
        {
            for (int x = xMin; x < xMax && x < m_iWidth; ++x)
            {
                real alpha = f(pCoords[1], pCoords[2], x, y) / fAlpha;
                real beta	= f(pCoords[2], pCoords[0], x, y) / fBeta;
                real gamma = f(pCoords[0], pCoords[1], x, y) / fGamma;


                // if the coordinates are positive, do the next check
                if (alpha >= 0 && beta >= 0 && gamma >= 0)
                {
                    double dZValue = alpha * pCoords[0][2] + beta * pCoords[1][2]  + gamma * pCoords[2][2];

                    if (dZValue < m_ZBuffer.at(x + y * m_iWidth))
                    {
                        QVector4D fragColor;

                        // interpolate all data
                        fragColor.setX(alpha * pColor[0][0] + beta * pColor[1][0] + gamma * pColor[2][0]);
                        fragColor.setY(alpha * pColor[0][1] + beta * pColor[1][1] + gamma * pColor[2][1]);
                        fragColor.setZ(alpha * pColor[0][2] + beta * pColor[1][2] + gamma * pColor[2][2]);
                        fragColor.setW(alpha * pColor[0][3] + beta * pColor[1][3] + gamma * pColor[2][3]);

                        m_ZBuffer[x + y * m_iWidth] = dZValue;

                        m_Image.setPixel(x, y, CQtHelper::toQColor(fragColor).rgba());
                    }
                }
            }
        }
    }

private:

    int m_iWidth;
    int m_iHeight;
    mutable QImage m_Image;
    mutable QVector<float> m_ZBuffer;
    QVector4D m_BackgroundColor;
};

#endif // CRASTERIZER_H
