#ifndef CARRAY2D_H
#define CARRAY2D_H

#include <QVector>
#include <QDebug>

//------------------------------------------------------------------------------------------------
// Array 2D
//------------------------------------------------------------------------------------------------
struct SPosition
{
    int x, y;
};

template<class T>
class CArray2D
{

public:

    //! Constructeur
    CArray2D() : m_iWidth(0), m_iHeight(0) {}

    //! Constructeur
    CArray2D(int width, int height)
        : m_iWidth(width), m_iHeight(height)
    {
        m_vData.resize(width*height);
    }

    //! Constructeur
    CArray2D(int width, int height, const T& initValue)
        : m_iWidth(width), m_iHeight(height)
    {
        m_vData.resize(width*height); m_vData.fill(initValue);
    }

    //! Redimensionne le tableau
    void resize(int width, int height)
    {
        m_iWidth = width;
        m_iHeight = height;
        m_vData.resize(width * height);
    }

    //! Retourne un élement
    T get(int i, int j)					{ return m_vData[i + j * m_iWidth]; }

    //! Retourne un élement
    const T& get(int i, int j) const	{ return m_vData[i + j * m_iWidth]; }

    //! Retourne un élement
    T get(int i) 						{ return m_vData[i]; }

    //! Retourne un élement
    const T& get(int i)	const			{ return m_vData[i]; }

    //! Définit un élément
    void set(int i, int j, const T& v)	{ m_vData[i + j * m_iWidth] = v; }

    //! Définit un élément
    void set(int i, const T& v)			{ m_vData[i] = v; }

    //! Retourne l'ensemble des élaments'
    const QVector<T>& get() const		{ return m_vData; }

    //! Retourne le nombre total d'éléments
    int size() const					{ return m_vData.size(); }

    //! Remplit l'ensemble du tableau
    void fill(const T& value)			{ m_vData.fill(value); }

    //! Retourne un sous ensemble du tableau
    CArray2D<T> getSub(int i, int j, int width, int height) const
    {
        CArray2D<T> subTab(width, height);

        for (int w = 0; w < width; ++w)
            for (int h = 0; h < height; ++h)
                subTab.set(w, h, get(i+w, j+h));

        return subTab;
    }

    //! Trouve le premier élement égal à value
    bool findFirst(SPosition& pos, T value) const
    {
        for (int h = 0; h < m_iHeight; ++h)
            for (int w = 0; w < m_iWidth; ++w)
                if (get(w, h) == value)
                {
                    pos.x = w;
                    pos.y = h;
                    return true;
                }

        return false;
    }

    //! Trouve l'ensemble des élements égal à value
    bool findAll(QVector<SPosition>& vecPos, T value) const
    {
        bool fFind = false;
        for (int h = 0; h < m_iHeight; ++h)
            for (int w = 0; w < m_iWidth; ++w)
                if (get(w, h) == value)
                {
                    SPosition pos;
                    pos.x = w;
                    pos.y = h;
                    vecPos << pos;
                }

        return fFind;
    }

    //! Affiche le tableau sur la sortie standard
    void dump() const
    {
        QString str;
        for (int h = 0; h < m_iHeight; ++h)
        {
            for (int w = 0; w < m_iWidth; ++w)
            {
                str += QString("%1 ").arg(get(w, h));
            }
            str += QString("\n");
        }
        qDebug() << str;
    }

    //! Copie un tableau dans celui-ci
    void copy(int i, int j, const CArray2D<T>& t)
    {
        for (int ii = 0; ii < t.width(); ++ii)
            for (int jj = 0; jj < t.height(); ++jj)
                set(ii+i, jj+j, t.get(ii, jj));
    }

    //! Copie un tableau dans celui-ci
    void copy(int i, int j, int ti, int tj, const CArray2D<T>& t)
    {
        for (int ii = 0; ii < t.width(); ++ii)
            for (int jj = 0; jj < t.height(); ++jj)
                set(ii+i, jj+j, t.get(ii+ti, jj+tj));
    }

    //! Retourne la largeur
    int width() const { return m_iWidth; }

    //! Retourne la hauteur
    int height() const { return m_iHeight; }

    //! Retourne l'item à la position i, j
    const T& operator()(int i, int j) const { return m_vData[i + j * m_iWidth]; }

    //! Retourne l'item à la position i, j
    T& operator()(int i, int j) { return m_vData[i + j * m_iWidth]; }

    //--------------------------------------------------------------------------
    //! Interpolation linéaire entre deux éléments
    real interpolate(int iX1, int iY1, int iX2, int iY2, real dX, real dY)
    {
        if (iX1 == iX2 && iY1 == iY2)
        {
            return get(iX1, iY1);
        }
        else if (iX1 == iX2)
        {
            return Math::interpolate(get(iX1, iY1), get(iX1, iY2), dY - iY1);
        }
        else if (iY1 == iY2)
        {
            return Math::interpolate(get(iX1, iY1), get(iX2, iY1), dX - iX1);
        }

        return (1. / (real)((iX2 - iX1) * (iY2 - iY1))) * (real)
                (get(iX1, iY1) * ((real)iX2 - dX) * ((real)iY2 - dY) +
                 get(iX2, iY1) * (dX - (real)iX1) * ((real)iY2 - dY) +
                 get(iX1, iY2) * ((real)iX2 - dX) * (dY - (real)iY1) +
                 get(iX2, iY2) * (dX - (real)iX1) * (dY - (real)iY1));

    }

    //--------------------------------------------------------------------------
    //! Retoiurne une copie du tableau redimensionné
    CArray2D<T> scaled(int iWidth, int iHeight)
    {
        CArray2D<T> result(iWidth, iHeight);

        real fWidthRatio = m_iWidth / (real)(iWidth + 1);
        real fHeightRatio = m_iHeight / (real)(iHeight + 1);

        for (int i = 0; i < iWidth; ++i)
        {
            real fI = fWidthRatio * i;
            for (int j = 0; j < iHeight; ++j)
            {
                real fJ = fHeightRatio * j;

                result(i, j) = interpolate(floor(fI), floor(fJ), ceil(fI), ceil(fJ), fI, fJ);
            }
        }

        return result;
    }


protected:

    //! Largeur, hauteur
    int m_iWidth, m_iHeight;

    //! Data
    QVector<T> m_vData;
};


#endif // CARRAY2D_H
