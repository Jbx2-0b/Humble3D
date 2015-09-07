#ifndef CARRAY3D_H
#define CARRAY3D_H

#include <QVector>
#include <QDebug>

//------------------------------------------------------------------------------------------------
// Array 3D
//------------------------------------------------------------------------------------------------
template<class T>
class CArray3D
{

public:

    //! Constructeur
    CArray3D()
        : m_iSizeX(0)
        , m_iSizeY(0)
        , m_iSizeZ(0) {}

    //! Constructeur
    CArray3D(int iSizeX, int iSizeY, int iSizeZ)
        : m_iSizeX(iSizeX)
        , m_iSizeY(iSizeY)
        , m_iSizeZ(iSizeZ)
    {
        m_vData.resize(iSizeX * iSizeY * iSizeZ);
    }

    //! Constructeur
    CArray3D(int iSizeX, int iSizeY, int iSizeZ, const T& initValue)
        : m_iSizeX(iSizeX)
        , m_iSizeY(iSizeY)
        , m_iSizeZ(iSizeZ)
    {
        m_vData.resize(iSizeX * iSizeY * iSizeZ);
        m_vData.fill(initValue);
    }

    //! Redimensionne le tableau
    void resize(int iSizeX, int iSizeY, int iSizeZ)
    {
        m_iSizeX = iSizeX;
        m_iSizeY = iSizeY;
        m_iSizeZ = iSizeZ;
        m_vData.resize(iSizeX * iSizeY * iSizeZ);
    }

    //! Retourne un élement
    T get(int i, int j, int k) const	{ return m_vData[i + j * m_iSizeX + k * m_iSizeX * m_iSizeY]; }

    //! Retourne un élement
    T get(int i) const					{ return m_vData[i]; }

    //! Définit un élément
    void set(int i, int j, int k, T v)	{ m_vData[i + j * m_iSizeX + k * m_iSizeX * m_iSizeY] = v; }

    //! Définit un élément
    void set(int i, T v)				{ m_vData[i] = v; }

    //! Retourne l'ensemble des éléments
    const QVector<T>& get() const		{ return m_vData; }

    //! Retourne le nombre total d'éléments
    int size() const					{ return m_vData.size(); }

    //! Remplit l'ensemble du tableau
    void fill(const T& value)			{ m_vData.fill(value); }


    //! Retourne le tableau sous forme de champ de bit
    virtual QByteArray toByteArray(int iCompressionFactor = 0) const
    {
        QByteArray uncompressData;
        QDataStream out(&uncompressData, QIODevice::WriteOnly);
        out << m_vData;
        out << m_iSizeX;
        out << m_iSizeY;
        out << m_iSizeZ;
        QByteArray compressData = qCompress(uncompressData, iCompressionFactor);
        return compressData;
    }

    //! Définit le tableau à partir d'un champ de bit
    virtual void fromByteArray(QByteArray* pBA)
    {
        QByteArray uncompressData = qUncompress(*pBA);
        QDataStream in(&uncompressData, QIODevice::ReadOnly);
        in >> m_vData;
        in >> m_iSizeX;
        in >> m_iSizeY;
        in >> m_iSizeY;
    }

    //! Affiche le tableau sur la sortie standard
    void dump() const
    {
        for (int x = 0; x < m_iSizeX; ++x)
        {
            for (int y = 0; y < m_iSizeY; ++y)
            {
                for (int z = 0; z < m_iSizeZ; ++z)
                {
                    qDebug() << get(x, y, z);
                }
            }
        }
    }

    //! Retourne la taille sur l'axe X
    int sizeX() const { return m_iSizeX; }

    //! Retourne la taille sur l'axe Y
    int sizeY() const { return m_iSizeY; }

    //! Retourne la taille sur l'axe Z
    int sizeZ() const { return m_iSizeZ; }

protected:

    int m_iSizeX, m_iSizeY, m_iSizeZ;

    //! Data
    QVector<T> m_vData;

};

#endif // CARRAY3D_H
