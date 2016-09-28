#ifndef CBINARYLOADER_H
#define CBINARYLOADER_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QFile>
#include <QDataStream>
#include <QByteArray>


class LIB_TOOLSSHARED_EXPORT CBinaryLoader
{

public:

    template<class T>
    inline static bool load(const QString& fileName, T* data);

    template<class T>
    inline static bool save(const QString& fileName, const T* data, int iCompressionFactor /*= 0*/);
};


template<class T>
bool CBinaryLoader::load(const QString& fileName, T* data)
{
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray compressData = file.readAll();
        file.close();
        QByteArray uncompressData = qUncompress(compressData);

        QDataStream in(&uncompressData, QIODevice::ReadOnly);

        in >> *data;

        return true;
    }

    return false;
}

template<class T>
bool CBinaryLoader::save(const QString& fileName, const T* data, int iCompressionFactor /*= 0*/)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        QByteArray uncompressData;
        QDataStream out(&uncompressData, QIODevice::WriteOnly);

        out << *data;

        QByteArray compressData = qCompress(uncompressData, iCompressionFactor);
        file.write(compressData, compressData.size());
        file.close();

        return true;
    }

    return false;
}


#endif // CBINARYLOADER_H
