#include "CConfigFileManager.h"

#include <QCoreApplication>


//-----------------------------------------------------------------------
CConfigFileManager::CConfigFileManager(const QString& sConfigFileName)
{
    QString fileName = QCoreApplication::applicationDirPath() + "/" + sConfigFileName;
    m_pSettings = new QSettings(fileName, QSettings::IniFormat);
}

//-----------------------------------------------------------------------
CConfigFileManager::~CConfigFileManager()
{

}

//-----------------------------------------------------------------------
QString CConfigFileManager::getString(const QString& groupId, const QString& stringId, const QString& defaultString)
{
    QString returnValue;

    m_pSettings->beginGroup(groupId);
    returnValue = m_pSettings->value(stringId, defaultString).toString();
    m_pSettings->endGroup();

    return returnValue;
}

//-----------------------------------------------------------------------
int CConfigFileManager::getInt(const QString& groupId, const QString& stringId, int defaultInt)
{
    int returnValue;

    m_pSettings->beginGroup(groupId);
    returnValue = m_pSettings->value(stringId, defaultInt).toInt();
    m_pSettings->endGroup();

    return returnValue;
}

//-----------------------------------------------------------------------
bool CConfigFileManager::getBoolean(const QString& groupId, const QString& stringId, bool defaultBool)
{
    bool returnValue;

    m_pSettings->beginGroup(groupId);
    returnValue = m_pSettings->value(stringId, defaultBool).toBool();
    m_pSettings->endGroup();

    return returnValue;
}

//-----------------------------------------------------------------------
double CConfigFileManager::getDouble(const QString& groupId, const QString& stringId, double defaultDouble)
{
    double returnValue;

    m_pSettings->beginGroup(groupId);
    returnValue = m_pSettings->value(stringId, defaultDouble).toDouble();
    m_pSettings->endGroup();

    return returnValue;
}

//-----------------------------------------------------------------------
void CConfigFileManager::setBoolean(const QString& groupId, const QString& stringId, bool bValue)
{
    m_pSettings->beginGroup(groupId);
    m_pSettings->setValue(stringId, bValue);
    m_pSettings->endGroup();
}

//-----------------------------------------------------------------------
void CConfigFileManager::setDouble(const QString& groupId, const QString& stringId, double dValue)
{
    m_pSettings->beginGroup(groupId);
    m_pSettings->setValue(stringId, dValue);
    m_pSettings->endGroup();
}
