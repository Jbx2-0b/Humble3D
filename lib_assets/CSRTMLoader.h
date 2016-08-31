#ifndef CSRTMLOADER_H
#define CSRTMLOADER_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CResourcesPathManager.h"
#include "CMapDictionary.h"
#include "CSRTMData.h"

// Qt
#include <QString>
#include <QHash>
#include <QTime>
#include <QFile>
#include <QTextStream>

static const int iHeightNoData = -150;

class LIB_ASSETS_SHARED_EXPORT CSRTMLoader
{
public:

    static CSRTMData<int> loadASCFile(const QString& filePathName)
    {
        QFile file(CResourcesPathManager::getInstance()->getFilePathName(filePathName));

        CSRTMData<int> data;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            pLog->addMessage(eERROR, "Impossible d'ouvrir " + filePathName);
            return data;
        }

        QTextStream in(&file);


        int iColumnCount = in.readLine().split(" ", QString::SkipEmptyParts)[1].toInt();
        int iRowCount = in.readLine().split(" ", QString::SkipEmptyParts)[1].toInt();
        data.setXCorner(in.readLine().split(" ", QString::SkipEmptyParts)[1].toDouble());
        data.setZCorner(in.readLine().split(" ", QString::SkipEmptyParts)[1].toDouble());
        data.setCellSize(in.readLine().split(" ", QString::SkipEmptyParts)[1].toDouble());
        int iNoData	= in.readLine().split(" ", QString::SkipEmptyParts)[1].toInt();

        pLog->addMessage(eINFO, "Columns count: "	+ QString::number(iColumnCount));
        pLog->addMessage(eINFO, "Rows count: "		+ QString::number(iRowCount));
        pLog->addMessage(eINFO, "XllCorner count: " + QString::number(data.getXCorner()));
        pLog->addMessage(eINFO, "YllCorner count: " + QString::number(data.getZCorner()));
        pLog->addMessage(eINFO, "Cell size: "		+ QString::number(data.getCellSize()));

        int iID = 0;

        data.resize(iColumnCount + 1, iRowCount + 1);

        QString word;

        QTime time;
        time.start();

        while(!in.atEnd())
        {
            in >> word;

            int iHeight = word.toInt();

            int x = iID % iColumnCount;
            int z = iID / iColumnCount;

            data.setHeight(x, z, iHeight == iNoData ? iHeightNoData : iHeight);

            iID++;
        }
        qDebug() << time.elapsed();

        return data;
    }

    static bool saveASCFile(const CSRTMData<int>& data, const QString& filePathName)
    {
        QFile file(filePathName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            pLog->addMessage(eERROR, "Impossible d'ouvrir " + filePathName);
            return false;
        }

        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << "ncols         " << data.width() << "\n";
        out << "nrows         " << data.height() << "\n";
        out << "xllcorner     " << data.getXCorner() << "\n";
        out << "yllcorner     " << data.getZCorner() << "\n";
        out << "cellsize      " << data.getCellSize() << "\n";
        out << "NODATA_value  -9999\n";


        for (int i = 0; i < data.width(); ++i)
        {
            for (int j = 0; j < data.height(); ++j)
            {
                out << QString::number(data.get(i, j)) << " ";
            }

            out << "\n";
        }

        file.close();

        return true;
    }

    static void toBinaryFile(const QString& filePathNameIn, const QString& filePathNameOut, int iCompressionFactor = 0)
    {
        CSRTMData<int> globalData = loadASCFile(filePathNameIn);
        globalData.saveBinary(filePathNameOut, iCompressionFactor);
    }

    static CMapDictionary* loadASCFile(const QString& filePathName, int iTerrainResolution, real fTerrainSize)
    {
        CSRTMData<int> globalData = loadASCFile(filePathName);
        return fromCSRTMData(globalData, iTerrainResolution, fTerrainSize);
    }

    static CMapDictionary* loadBinaryFile(const QString& filePathName, int iTerrainResolution, real fTerrainSize)
    {
        CSRTMData<int> globalData;
        globalData.loadBinary(filePathName);
        return fromCSRTMData(globalData, iTerrainResolution, fTerrainSize);
    }

    static CSRTMData<int> extractSRTMData(CMapDictionary* pDictionary, const QRect& rect, real dStep)
    {
        int iWidth = rect.width() / dStep;
        int iHeight = rect.height() / dStep;

        CSRTMData<int> data(iWidth, iHeight, 0);

        data.setXCorner(0.);
        data.setZCorner(0.);
        data.setCellSize(CMapDictionary::toDeg(dStep));

        for (int i = 0; i < iWidth; ++i)
        {
            for (int j = 0; j < iHeight; ++j)
            {
                data.setHeight(i, j, (int) (pDictionary->getAbsoluteHeight(rect.x() + i * dStep, rect.y() + j *dStep)));
            }
        }

        return data;
    }

    static bool toASCFile(const QString& filePathName, CMapDictionary* pDictionary, const QRect& rect, real dStep)
    {
        CSRTMData<int> data = extractSRTMData(pDictionary, rect, dStep);
        return saveASCFile(data, filePathName);
    }

private:

    static CMapDictionary* fromCSRTMData(const CSRTMData<int>& globalData, int iTerrainResolution, real fTerrainSize)
    {
        CMapDictionary* pMapDictionary = new CMapDictionary(globalData.width(), globalData.height(), iTerrainResolution);
        pMapDictionary->setXCorner(globalData.getXCorner());
        pMapDictionary->setZCorner(globalData.getZCorner());
        pMapDictionary->setCellSize(globalData.getCellSize());
        pMapDictionary->setMapSize(fTerrainSize);

        pMapDictionary->setHeights(globalData);

        int keyI = 0;

        for (int i = 0; i < globalData.width(); i += iTerrainResolution)
        {
            int keyJ = 0;
            for (int j = 0; j < globalData.height(); j += iTerrainResolution)
            {
                CMap* pMap = pMapDictionary->createMap(CMapKey(keyI, keyJ));
                pMap->setPolygonCount((iTerrainResolution - 1) * (iTerrainResolution - 1) * 2);

                real fMapSize = pMapDictionary->getMapSize();
                QVector3D min = pMapDictionary->getPosition() + QVector3D(keyI * fMapSize, 0., keyJ * fMapSize);
                QVector3D max = min + QVector3D(fMapSize, fMapSize, fMapSize);
                pMap->setBoundingBox(CBox3D(min, max));
                keyJ++;
            }
            keyI++;
        }

        return pMapDictionary;
    }
};

#endif // CSRTMLOADER_H
