#ifndef CBUFFER_H
#define CBUFFER_H

// Gestionnaire de log
#include "CLogManager.h"

// Qt
#include <QVector>

template <class DataType>
class CBuffer : public QVector<DataType>
{
public:

    //! Constructeur
    CBuffer()
        : QVector<DataType>()
        , m_bIsDirty(false)
    {}

    //! Constructeur
    CBuffer(int iSize)
        : QVector<DataType>(iSize)
        , m_bIsDirty(false)
    {}

    //! Constructeur
    CBuffer(int iSize, const DataType & value)
        : QVector<DataType>(iSize, value)
        , m_bIsDirty(false)
    {}

    CBuffer(const CBuffer& buffer)
        : QVector<DataType>(buffer)
        , m_bIsDirty(buffer.m_bIsDirty)
    {
    }

    CBuffer& operator=(const CBuffer& buffer)
    {
        QVector<DataType>::operator =(buffer);
        m_bIsDirty = buffer.m_bIsDirty;
        return *this;
    }

    //! Retourne la taille en mémoire du buffer
    inline unsigned int getSize() const
    {
        int iCount = QVector<DataType>::count();
        int iDataTypeSize = sizeof(DataType);

        return iCount * iDataTypeSize;
    }

    //! Retourne l'état du buffer
    bool isDirty() const { return m_bIsDirty; }

    //! Définit l'état du buffer
    void setDirty()		{ m_bIsDirty = true; }

    //! Définit l'état du buffer
    void wash()	const	{ m_bIsDirty = false; }

private:

    mutable bool m_bIsDirty;
};

class CUserBuffer : public CBuffer<float>
{
public:
    //! Constructeur
    CUserBuffer() : CBuffer<float>(), m_iTupleSize(1) {}

    //! Constructeur
    CUserBuffer(int iSize) : CBuffer<float>(iSize), m_iTupleSize(1) {}

    //! Constructeur
    CUserBuffer(int iSize, const real& value)
        : CBuffer<float>(iSize, value), m_iTupleSize(1) {}

    //! Définit la taille d'un tuple
    void setTupleSize(int iTupleSize)
    {
        if (iTupleSize > 4)
        {
            LogManager.addMessage(eERROR, "Impossible de spécifier des tuples > 4");
            return;
        }

        m_iTupleSize = iTupleSize;
    }

    //! Retourne la taille d'un tuple
    int getTupleSize() const { return m_iTupleSize; }

protected:

    int m_iTupleSize;
};

#endif // CBUFFER_H
