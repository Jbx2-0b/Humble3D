#ifndef CARRAY_H
#define CARRAY_H

// Qt
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QDataStream>


template <class Type, int iSize>
class CArray
{
public:

    CArray() { memset(&m_array, 0, sizeof(m_array));	}

    inline void set(unsigned int iID, const Type& value) { m_array[iID] = value; }

    inline int get(unsigned int iID) const { return m_array[iID]; }

    inline Type& operator[](unsigned int i) { return m_array[i]; }

    inline Type* data() { return m_array; }

    inline const Type* constData() const { return m_array; }

private:

    Type m_array[iSize];
};




#endif // CARRAY_H
