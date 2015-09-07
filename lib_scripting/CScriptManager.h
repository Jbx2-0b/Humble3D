#ifndef CSCRIPTMANAGER_H
#define CSCRIPTMANAGER_H

// Lib
#include "lib_scripting_global.h"

// Geometry
#include "CSceneManager.h"

// Xml
#include "CXmlReader.h"
#include "CXmlWriter.h"


class LIB_SCRIPTINGSHARED_EXPORT CScriptManager
{
public:

    //! Constructeur
    CScriptManager(CSceneManager* pSceneManager);

    //! Enregistre un type scriptable
    template<typename Scriptable> void registerScriptable(const QString& typeName)
    {
        m_pSceneManager->registerScriptable<Scriptable>(typeName);
    }

    //! Charge une scene
    bool loadScript(const QString& fileName, CSceneNode* pNode);

    //! Sauvegarde une scene
    bool saveScript(const QString& fileName);



private:
    void recursiveLoad(CXmlElement* pElement, CSceneNode* pCurrentNode);
    void recursiveSave(CXmlElement* pElement, CSceneNode* pCurrentNode);

    void saveData();
    QString path(const QString& fileName);

    CSceneManager* m_pSceneManager;

    QMap<QString, QString> m_MeshNames;

    bool m_bSaveOnlyUsed;
    bool m_bSaveData;

    QString m_CurrentPath;
};

#endif // CSCRIPTMANAGER_H
