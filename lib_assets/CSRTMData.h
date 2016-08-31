#ifndef CSRTMDATA_H
#define CSRTMDATA_H


// Foundations
#include "CArray2D.h"
#include "CResourcesPathManager.h"

// Qt
#include <QFile>


template<class T>
class CSRTMData : public CArray2D<T>
{
public:

    CSRTMData()
        : CArray2D<T>()
        , m_fXllCorner(0.)
        , m_fZllCorner(0.)
        , m_fCellSize(0.)
        , m_HeightMax(0)
    {}

    CSRTMData(int width, int height)
        : CArray2D<T>(width, height)
        , m_fXllCorner(0.)
        , m_fZllCorner(0.)
        , m_fCellSize(0.)
        , m_HeightMax(0)
    {}

    CSRTMData(int width, int height, T initValue)
        : CArray2D<T>(width, height, initValue)
        , m_fXllCorner(0.)
        , m_fZllCorner(0.)
        , m_fCellSize(0.)
        , m_HeightMax(0)
    {}

    void saveBinary(const QString& filePathName, int iCompressionFactor = 0) const
    {
        QByteArray binData = toByteArray(iCompressionFactor);

        QFile file(filePathName);

        if (file.open(QIODevice::WriteOnly))
        {
            file.write(binData, binData.size());
            file.close();
        }
    }

    void loadBinary(const QString& filePathName)
    {
        QFile file(CResourcesPathManager::getInstance()->getFilePathName(filePathName));

        if (file.open(QIODevice::ReadOnly))
        {
            fromByteArray(file.readAll());
            file.close();
        }
    }

    virtual QByteArray toByteArray(int iCompressionFactor = 0) const
    {
        QByteArray uncompressData;
        QDataStream out(&uncompressData, QIODevice::WriteOnly);
        out << CArray2D<T>::m_vData;
        out << CArray2D<T>::m_iWidth;
        out << CArray2D<T>::m_iHeight;
        out << m_fXllCorner;
        out << m_fZllCorner;
        out << m_fCellSize;
        out << m_HeightMax;
        QByteArray compressData = qCompress(uncompressData, iCompressionFactor);
        return compressData;
    }

    virtual void fromByteArray(const QByteArray& pBA)
    {
        QByteArray uncompressData = qUncompress(pBA);
        QDataStream in(&uncompressData, QIODevice::ReadOnly);
        in >> CArray2D<T>::m_vData;
        in >> CArray2D<T>::m_iWidth;
        in >> CArray2D<T>::m_iHeight;
        in >> m_fXllCorner;
        in >> m_fZllCorner;
        in >> m_fCellSize;
        in >> m_HeightMax;
    }

    void setXCorner(real fXCorner) { m_fXllCorner = fXCorner; }
    void setZCorner(real fZCorner) { m_fZllCorner = fZCorner; }
    void setCellSize(real fSize)	{ m_fCellSize = fSize; }

    void setHeight(int iX, int iY, T altitude)
    {
        CArray2D<T>::set(iX, iY, altitude);

        if (altitude > m_HeightMax) m_HeightMax = altitude;
    }

    real getXCorner() const	{ return m_fXllCorner; }
    real getZCorner() const	{ return m_fZllCorner; }
    real getCellSize() const	{ return m_fCellSize; }
    T getHeightMax() const		{ return m_HeightMax; }


protected:

    real m_fXllCorner;
    real m_fZllCorner;
    real m_fCellSize;
    T m_HeightMax;
};



#endif // CSRTMDATA_H
