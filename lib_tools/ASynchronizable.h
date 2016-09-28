#ifndef ASYNCHRONIZABLE_H
#define ASYNCHRONIZABLE_H

// Qt
#include <QReadWriteLock>

#ifdef DEBUGLOCK
	#include <QDebug>
	#define MAXLOCK 5
#endif


class ASynchronizable
{
public:

	//! Constructeur
	ASynchronizable() 
		: m_Lock(QReadWriteLock::Recursive)
#ifdef DEBUGLOCK
		, m_iLockCount(0)
#endif
	{}

	//! Constructeur de recopie
	ASynchronizable(const ASynchronizable&) {}

	//! Destructeur
    virtual ~ASynchronizable() {}

	//! Lock le buffer en lecture
	void lockForRead() const
	{ 
		m_Lock.lockForRead();

#ifdef DEBUGLOCK
		m_iLockCount++;
		if (m_iLockCount > MAXLOCK)
		{
			Q_ASSERT(false);
		}
#endif
	}

	//! Lock le buffer en écriture
	void lockForWrite()	const						
	{ 
		m_Lock.lockForWrite();
#ifdef DEBUGLOCK
		m_iLockCount++; 
		if (m_iLockCount > MAXLOCK)
		{
			Q_ASSERT(false);
		}
#endif
	}

	//! Unlock le buffer
	void unlock() const
	{ 
		m_Lock.unlock();
#ifdef DEBUGLOCK
		m_iLockCount--;
#endif
	}
	
protected:

#ifdef DEBUGLOCK
	mutable int m_iLockCount;
#endif

	//! Mutex
	mutable QReadWriteLock m_Lock;
};


#endif // ASYNCHRONIZABLE_H
