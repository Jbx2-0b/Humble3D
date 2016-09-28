#include "CLogManager.h"

#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

const char* CLogManager::s_LogTypeNames[] = { "FATAL", "ERROR", "WARN", "DEBUG", "DEBUGMEMORY", "DEBUGTHREAD", "INFO" };

//-----------------------------------------------------------------------------------------
CLogManager::CLogManager()
{
    QFile file(m_FileName.isEmpty() ? defaultLogFileName : m_FileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le fichier de log !";
    }

    m_ActiveLogTypes[eFATAL]		= true;
    m_ActiveLogTypes[eERROR]		= true;
    m_ActiveLogTypes[eWARN]			= true;
    m_ActiveLogTypes[eDEBUG]		= true;
    m_ActiveLogTypes[eDEBUGMEMORY]	= false;
    m_ActiveLogTypes[eDEBUGTHREAD]	= false;
    m_ActiveLogTypes[eINFO]			= true;
}

//-----------------------------------------------------------------------------------------
CLogManager::~CLogManager()
{
}

//-----------------------------------------------------------------------------------------
CLogManager &CLogManager::getInstance()
{
    static CLogManager instance;
    return instance;
}

//-----------------------------------------------------------------------------------------
void CLogManager::registerListener(ILogManagerListener* pListener)
{
    m_LogManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CLogManager::unregisterListener(ILogManagerListener* pListener)
{
    m_LogManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CLogManager::setFileName(const QString& fileName)
{
    m_FileName = fileName;
}

//-----------------------------------------------------------------------------------------
void CLogManager::setActive(EnumLogType eType, bool bVisible)
{
    m_ActiveLogTypes[eType] = bVisible;
}

//-----------------------------------------------------------------------------------------
void CLogManager::addMessage(EnumLogType eType, const QString& msg)
{
    if (msg.isEmpty())
        return;

    if (!m_ActiveLogTypes[eType])
    {
        return;
    }

    QString trimmedMsg = msg.trimmed();

    qDebug() << msg;

    QString newLine = QString(s_LogTypeNames[eType]) + "\t" + QTime::currentTime().toString("hh:mm:ss.zzz") + "\t" + trimmedMsg;
    m_listMsg.append(newLine);
    m_mapMsg.insert(eType, msg);

    QFile file(m_FileName.isEmpty() ? defaultLogFileName : m_FileName);

    if (!file.open(QIODevice::Append  | QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Impossible de charger le fichier de log !";
        return;
    }

    QTextStream out(&file);
    out << newLine + "\n";
    file.close();

    notifyNewMessage(eType, trimmedMsg);
}

