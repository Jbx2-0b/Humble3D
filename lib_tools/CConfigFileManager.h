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

    //! Retourne une chaine de caractère correspondant à l'ID donné
    QString getString(const QString& groupId, const QString& stringId, const QString& defaultString);

    //! Retourne un entier correspondant à l'ID donné
    int getInt(const QString& groupId, const QString& stringId, int defaultInt);

    //! Retourne un booléen correspondant à l'ID donné
    bool getBoolean(const QString& groupId, const QString& stringId, bool defaultBool);

    //! Retourne un double correspondant à l'ID donné
    double getDouble(const QString& groupId, const QString& stringId, double defaultDouble);

    //! Définit la propriété stringId du groupe groupId
    void setBoolean(const QString& groupId, const QString& stringId, bool bValue);

    //! Définit la propriété stringId du groupe groupId
    void setDouble(const QString& groupId, const QString& stringId, double dValue);

protected:

    QSettings* m_pSettings;
};

#endif // CCONFIGFILEMANAGER_H
