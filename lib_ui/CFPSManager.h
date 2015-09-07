#ifndef CFPSMANAGER_H
#define CFPSMANAGER_H

// Foundations
#include "CMath.h"
#include "CGeometryGlobal.h"

// Qt
#include <QTimer>
#include <QTime>


class CFPSManager : public QObject
{
	Q_OBJECT

public:

	//! Constructeur
	CFPSManager()
        : m_iDesiredFPS(DefaultDesiredFPS)
		, m_iFPSCount(0)
		, m_iLastFPSCount(0)
	{
		m_pTimer = new QTimer(this);
        m_pTimer->setInterval(1000. / m_iDesiredFPS);
		connect(m_pTimer, SIGNAL(timeout()), this, SIGNAL(FPSTimeout()));
		m_LastTime = QTime::currentTime();
	}

    virtual ~CFPSManager()
    {
        delete m_pTimer;
    }

    //! Démarre le gestionnaire de FPS
    void start() { m_pTimer->start(); }

    //! Arrête le gestionnaire de FPS
    void stop() { m_pTimer->stop(); }

	//! Définit le nombre de FPS désirées - 0 pour le maximum
	virtual void setDesiredFPS(int iDesiredFPS)
	{ 
		m_iDesiredFPS = iDesiredFPS;
		m_pTimer->setInterval(iDesiredFPS = 0 ? 0 : 1000. / (real)iDesiredFPS);
	}

	//! Retourne le nombre de FPS désirées
	int getDesiredFPS() const { return m_iDesiredFPS; }

	//! Incrémente les FPS
	void incrFPS()
	{
		// FPS count
		++m_iFPSCount;
		QTime newTime = QTime::currentTime();

		// if 1 sec (1000 msec) ellapsed
		if (m_LastTime.msecsTo(newTime) >= 1000)
		{
			m_iLastFPSCount = m_iFPSCount;
			m_iFPSCount = 0;
			m_LastTime = QTime::currentTime();

			emit FPSChanged(m_iLastFPSCount);
		}
	}

	//! Retourne le nombre de FPS courantes
	int getFPSCount()
	{
		return m_iLastFPSCount;
	}

signals:

	void FPSTimeout();
	void FPSChanged(int iFPS);

private:

	//! Définit le nombre de FPS désirée
	int m_iDesiredFPS;

	QTimer* m_pTimer;

	//! Permet de compter les FPS
	QTime m_LastTime;

	//! Nombre de frame courant
	int m_iFPSCount;

	//! Dernier nombre de frame
	int m_iLastFPSCount;
};


#endif // CFPSMANAGER_H
