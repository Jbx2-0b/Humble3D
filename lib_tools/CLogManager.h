#ifndef CLOGMANAGER_H
#define CLOGMANAGER_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QString>
#include <QStringList>
#include <QSet>
#include <QFile>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>

#define pLog CLogManager::getInstance()

const QString defaultLogFileName = "app.log";

//! Type de message
enum EnumLogType
{
	eFATAL,
	eERROR,
	eWARN,
	eDEBUG,
	eDEBUGMEMORY,
    eDEBUGTHREAD,
	eINFO
};


class LIB_TOOLSSHARED_EXPORT CLogManager
{
    class ILogManagerListener
	{
		friend class CLogManager;

	protected:

		virtual void onNewMessage(EnumLogType eType, const QString& message) = 0;
	};


public:

	//! Accesseur a l'instance de la classe.
	static CLogManager* getInstance()
	{
		if (s_pInstance == 0)
		{
			QMutexLocker locker(&s_Mutex);

			if (s_pInstance == 0)
			{
				s_pInstance = new CLogManager();
			}
		}

		return s_pInstance;
    }

	//! Destructeur de l'instance de la classe.
	static void killInstance()
	{
		if (s_pInstance)
		{
			QMutexLocker locker(&s_Mutex);
			delete s_pInstance;
			s_pInstance = 0;
		}
    }

	//! Enregistre un listener
    void registerListener(ILogManagerListener* pListener);

	//! Libére un listener
    void unregisterListener(ILogManagerListener* pListener);

    //!  Définit le nom du fichier de log
    void setFileName(const QString& fileName);

	//! Définit si le canal est actif
	void setActive(EnumLogType eType, bool bVisible);

	//! Récupére la liste des messages
    const QStringList& getAllMessages()	const { return m_listMsg; }

    //! Récupére la liste des messages en fonction de leur type
    QList<QString> getMessagesByType(EnumLogType eType) const { return m_mapMsg.values(eType); }

	//! Ajoute un message
    void addMessage(EnumLogType eType, const QString & msg);

protected:

	//! Constructeur
    CLogManager();

	//! Destructeur
    virtual ~CLogManager();

	//! Liste des messages
    QStringList m_listMsg;

    //! Liste des messages par type
    QMultiHash<EnumLogType, QString> m_mapMsg;

	//! Canaux actif
	QMap<EnumLogType, bool> m_ActiveLogTypes;

    static const char* s_LogTypeNames[];

	void notifyNewMessage(EnumLogType eType, const QString& message)
	{
        foreach (ILogManagerListener* pListener, m_LogManagerListeners)
		{
			pListener->onNewMessage(eType, message);
		}
	}

	//! Liste des listeners
    QSet<ILogManagerListener*> m_LogManagerListeners;

    QString m_FileName;

	static CLogManager* s_pInstance;
	static QMutex s_Mutex;
};

#endif // CLOGMANAGER_H
