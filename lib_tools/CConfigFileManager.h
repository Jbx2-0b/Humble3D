#ifndef CCONFIGFILEMANAGER_H
#define CCONFIGFILEMANAGER_H

// Lib
#include "lib_tools_global.h"

// Qt
#include <QString>
#include <QSettings>
#include <QMutex>
#include <QMutexLocker>


class LIB_TOOLSSHARED_EXPORT CConfigFileManager
{

public:

    //! Ctor
    CConfigFileManager(const QString& sConfigFileName);

    //! Dtor
    virtual ~CConfigFileManager();

    //! Retourne une chaine de caract�re correspondant � l'ID donn�
    QString getString(const QString& groupId, const QString& stringId, const QString& defaultString);

    //! Retourne un entier correspondant � l'ID donn�
    int getInt(const QString& groupId, const QString& stringId, int defaultInt);

    //! Retourne un bool�en correspondant � l'ID donn�
    bool getBoolean(const QString& groupId, const QString& stringId, bool defaultBool);

    //! Retourne un double correspondant � l'ID donn�
    double getDouble(const QString& groupId, const QString& stringId, double defaultDouble);

    //! D�finit la propri�t� stringId du groupe groupId
    void setBoolean(const QString& groupId, const QString& stringId, bool bValue);

    //! D�finit la propri�t� stringId du groupe groupId
    void setDouble(const QString& groupId, const QString& stringId, double dValue);

protected:

    QSettings* m_pSettings;
};

#endif // CCONFIGFILEMANAGER_H
