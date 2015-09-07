#ifndef CSERIALIZABLEARRAY2D_H
#define CSERIALIZABLEARRAY2D_H

#include "CArray2D.h"

template<class T>
class CSerializableArray2D : public CArray2D<T>
{
public:
	//! Retourne le tableau sous forme de champ de bit
	virtual QByteArray toByteArray(int iCompressionFactor = 0) const
	{
		QByteArray uncompressData;
		QDataStream out(&uncompressData, QIODevice::WriteOnly);
		out << m_vData;
		out << m_iWidth;
		out << m_iHeight;
		QByteArray compressData = qCompress(uncompressData, iCompressionFactor);
		return compressData;
	}

	//! Définit le tableau à partir d'un champ de bit
	virtual void fromByteArray(QByteArray* pBA)
	{
		QByteArray uncompressData = qUncompress(*pBA);
		QDataStream in(&uncompressData, QIODevice::ReadOnly);
		in >> m_vData;
		in >> m_iWidth;
		in >> m_iHeight;
	}
};

#endif // CSERIALIZABLEARRAY2D_H