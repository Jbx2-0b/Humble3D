#ifndef CSHADERMANAGER_H
#define CSHADERMANAGER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CShader.h"
#include "CGeometryGlobal.h"

// Qt
#include <QMutex>
#include <QMutexLocker>

class LIB_CORE_SHARED_EXPORT CShaderManager : public CShader::IShaderListener
{
public:

    //! Accesseur a l'instance de la classe.
    static CShaderManager* getInstance()
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CShaderManager();
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

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IShaderManagerListener
    {
    public:

        //! Prévient de l'ajout ou de la mise à jour d'un shader
        virtual void onUpdateShader(CShader* pShader) = 0;

        //! Prévient de la suppression d'un Shader
        virtual void onDeleteShader(CShader* pShader) = 0;
    };

    //! Enregistre un listener
    void registerListener(IShaderManagerListener* pListener);

    //! Libére un listener
    void unregisterListener(IShaderManagerListener* pListener);

    //! Retourne le nombre de shaders
    int getShaderCount() const												{ return m_Shaders.size(); }

    //! Retourne la collection de shaders
    const QList<CShader*> getShaders() const								{ return m_Shaders.values(); }

    //! Retourne la collection des de Shaders disponible
    const QList<QString> getShadersNames() const							{ return m_Shaders.keys(); }

    //! Retourne un hader en fonction de son nom
    CShader* getShaderByName(const QString& name) const;

    //! Retourne un shader en fonction de son id
    CShader* getShaderByID(int iID) const;

    //! Retourne true si le shader existe
    bool isShaderExist(const QString& name) const;

    //! Retourne true si le shader existe
    bool isShaderExist(int iID) const;

    //! Crée un nouveau shader et retourne son pointeur
    CShader* createShader(
            const QString& name,
            const QString& vertexShaderFileName = "",
            const QString& geometryShaderFileName = "",
            const QString& fragmentShaderFileName = "");

    //! Crée une nouveau shader et retourne son pointeur
    CShader* createShaderFromSourceCode(
            const QString& name,
            const QString& vertexShaderFileCode = "",
            const QString& geometryShaderFileCode = "",
            const QString& fragmentShaderFileCode = "");

    //! Supprime une Shader
    void removeShader(CShader* pShader);

    //! Libére l'ensemble des Shaders du manager
    void clearShaders();

protected:

    //! Implémentation de CShader::IShaderListener
    virtual void onUpdate(CShader* pShader);

    //! Implémentation de CShader::IShaderListener
    virtual void onDelete(CShader* pShader);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<IShaderManagerListener*> m_ShaderManagerListeners;

    //! Notifie les listeners qu'un shader a été ajouté
    void notifyUpdate(CShader* pShader);

    //! Notifie les listeners qu'un shader a été supprimé
    void notifyDelete(CShader* pShader);

private:

    //! Constructeur
    CShaderManager();

    //! Destructeur
    virtual ~CShaderManager();

    //! Conteneur des shaders
    QMap<QString, CShader*> m_Shaders;

    //! Conteneur des shaders en fonction de leur IDs
    QMap<int, CShader*> m_ShaderIDs;

    static CShaderManager* s_pInstance;
    static QMutex s_Mutex;
};

#endif // CSHADERMANAGER_H
