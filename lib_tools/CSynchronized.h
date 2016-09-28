#ifndef CSYNCHRONIZED_H
#define CSYNCHRONIZED_H

#include "ASynchronizable.h"

// Qt
#include <QVector>
#include <QList>
#include <QMap>
#include <QHash>

template <class T>
class CSynchronized : public T, public ASynchronizable {};

template <class T>
class CSynchronizedList : public QList<T>, public ASynchronizable
{
public:

	void append(const T& t)
	{
		lockForWrite();
		QList<T>::append(t);
		unlock();
	}

	void removeOne(const T& t)
	{
		lockForWrite();
		QList<T>::removeOne(t);
		unlock();
	}

	void removeAll(const T& t)
	{
		lockForWrite();
		QList<T>::removeAll(t);
		unlock();
	}
};

template <class U, class V>
class CSynchronizedHash : public QHash<U, V>, public ASynchronizable {};



#endif // CSYNCHRONIZED_H