#ifndef CLINEGRID_H
#define CLINEGRID_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"


static const real DefaultLineGridSize = 20.;
static const int DefaultLineGridSquareCount = 40;

class LIB_ASSETS_SHARED_EXPORT CLineGrid : public CMesh
{

public:

    //! Constructeur
    CLineGrid(real dLength = DefaultLineGridSize,
              real dWidth =  DefaultLineGridSize,
              int iLengthSquareCount = DefaultLineGridSquareCount,
              int iWidthSquareCount = DefaultLineGridSquareCount);

    //! Destructeur
    virtual ~CLineGrid();

    void setSize(real dLength, real dWidth, int iLengthSquareCount, int iWidthSquareCount)
    {
        m_dLength = dLength;
        m_dWidth = dWidth;
        m_iLengthSquareCount = iLengthSquareCount;
        m_iWidthSquareCount = iWidthSquareCount;

        update();
    }

protected:

    virtual void updateGeometry();

private:

    CSubMesh* m_pGridBuffer;
    real m_dLength;
    real m_dWidth;
    int m_iLengthSquareCount;
    int m_iWidthSquareCount;
};

#endif // CLINEGRID_H
