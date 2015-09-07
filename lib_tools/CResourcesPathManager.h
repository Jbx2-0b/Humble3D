#ifndef CRESOURCESPATHMANAGER_H
#define CRESOURCESPATHMANAGER_H

// Lib
#include "lib_tools_global.h"

// Foundations
#include "CLogManager.h"

// Qt
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QList>
#include <QDirIterator>
#include <QDebug>


//! Type de message
enum EnumPathType
{
    eAbsolute,
    eRelative,
};


class LIB_TOOLSSHARED_EXPORT CResourcesPathManager
{

public:

    //! Accesseur a l'instance de la classe.
    static CResourcesPathManager* getInstance()
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CResourcesPathManager();
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

    void setApplicationPath(const QString& sAppPath)
    {
        pLog->addMessage(eINFO, "Application path " + sAppPath);
        m_sAppPath = sAppPath;
        addPath(m_sAppPath, eAbsolute);

#ifdef EMBEDDED_TARGET
        addPath(":/assets/", eAbsolute);
#endif
    }

    void addPath(const QString& sPathName, EnumPathType ePathType = eRelative)
    {
        bool bSlashEnded = sPathName.endsWith('/');

        if (sPathName.size() > 0)
        {
            if (ePathType == eRelative)
            {
                if (sPathName[0] == '/')
                    m_sPaths << (m_sAppPath + sPathName + (bSlashEnded ? "" : "/"));
                else
                    m_sPaths << (m_sAppPath + "/" + sPathName + (bSlashEnded ? "" : "/"));
            }
            else
            {
                m_sPaths << (sPathName + (bSlashEnded ? "" : "/"));
            }
        }
    }

    void listResourcesFiles() const
    {
        qDebug() << "Resources Files :";

        foreach (const QString& sPath, m_sPaths)
        {
            qDebug() << sPath;
            QDirIterator dirIterator(sPath);
            while (dirIterator.hasNext())
            {
                qDebug() << dirIterator.next();
            }
        }

        qDebug() << "End.";
    }

    void listFiles(const QString& pathName) const
    {
        qDebug() << "List " + pathName + ":";

        QDirIterator dirIterator(pathName);
        while (dirIterator.hasNext())
        {
            qDebug() << dirIterator.next();
        }

        qDebug() << "End list " + pathName + ".";
    }

    bool isExist(const QString& filePathName) const
    {
        return QFileInfo(filePathName).exists();
    }

    QString getPathName(const QString& resourceName) const
    {
        QFileInfo fileInfo(resourceName);
        if (fileInfo.exists())
        {
            return fileInfo.absolutePath();
        }

        foreach (const QString& sPath, m_sPaths)
        {
            if (QFileInfo(sPath + resourceName).exists())
            {
                return sPath;
            }
        }

        return QString();
    }

    QString getFilePathName(const QString& resourceName) const
    {
        QFileInfo fileInfo(resourceName);
        if (fileInfo.exists())
        {
            return resourceName;
        }

        QString path = getPathName(resourceName);

        if (path.isEmpty()) return resourceName;

        return path + resourceName;
    }

protected:

    CResourcesPathManager(){}
    ~CResourcesPathManager(){}

    QString m_sAppPath;
    QList<QString> m_sPaths;
    static CResourcesPathManager* s_pInstance;
    static QMutex s_Mutex;
};


#endif // CRESOURCESPATHMANAGER_H
