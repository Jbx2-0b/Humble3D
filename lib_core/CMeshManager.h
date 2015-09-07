#ifndef CMESHMANAGER_H
#define CMESHMANAGER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"
#include "CSubMesh.h"
#include "CGeometryGlobal.h"
#include "CRegisterFactory.h"

// Qt
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>


class LIB_CORE_SHARED_EXPORT CMeshManager : public CMesh::IMeshListener
{

public:

    //! Accesseur de l'instance de la classe.
    static CMeshManager* getInstance()
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CMeshManager();
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

    class IMeshManagerListener
    {
    public:

        //! Prévient de l'ajout ou de la mise à jour d'un mesh
        virtual void onUpdate(CMesh* pMesh) = 0;

        //! Prévient de la suppression d'un mesh
        virtual void onDelete(CMesh* pMesh) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMeshManagerListener* pListener);

    //! Libére un listener
    void unregisterListener(IMeshManagerListener* pListener);

    class IMeshBufferListener
    {
        friend class CMeshManager;

    protected:
        //! Prévient de la suppression d'un VB
        virtual void onDeleteMeshBuffer(CMeshBuffer* pBuffer) = 0;
    };

    //! Enregistre un listener
    void registerMeshBufferListener(IMeshBufferListener* pListener);

    //! Libére un listener
    void unregisterMeshBufferListener(IMeshBufferListener* pListener);

    //! Retourne le nombre de meshs
    int getMeshCount() const											{ return m_MeshByName.size(); }

    //! Retourne la collection de meshs
    const QList<CMesh*> getMeshs() const                                { return m_MeshByName.values(); }

    //! Retourne la collection des noms des meshs disponibles
    const QList<QString> getMeshsNames() const                          { return m_MeshByName.keys(); }

    //! Retourne un mesh en fonction de son nom
    CMesh* getMeshByName(const QString& name) const;

    //! Retourne un mesh en fonction de son nom
    CMesh* getMeshByID(int iID) const;

    //! Retourne true si le mesh existe
    bool isMeshExist(const QString& name) const;

    //! Retourne true si le mesh existe
    bool isMeshExist(int iID) const;

    //! Crée un nouveau mesh
    CMesh* createMesh(const QString& name = "Mesh");

    //! Charge un mesh
    CMesh* loadMesh(const QString& fileName);

    //! Sauvegarde un mesh
    bool saveMesh(CMesh* pMesh, const QString& fileName, int iCompressionFactor);

    //! Charge un groupe de mesh
    CMeshGroup loadMeshGroup(const QString& fileName);

    //! Sauvegarde un groupe de mesh
    bool saveMeshGroup(const CMeshGroup& meshGroup, const QString& fileName, int iCompressionFactor);

    //! Supprime un mesh
    void removeMesh(CMesh* pMesh);

    //! Libére l'ensemble des meshs du manager
    void clearMeshs();

    //-------------------------------------------------------------------------------------------------
    // Gestion des types personnalisés
    //-------------------------------------------------------------------------------------------------

    //! Enregistre un type de mesh personnalisé
    template<typename CustomMesh> void registerCustomMesh(const QString& typeName);

    //! Crée un type de mesh personnalisé
    template<typename CustomMesh> CustomMesh* createCustomMesh(const QString& typeName, const QString& itemName = "");

protected:

    //! Fabrique pour les objets personnalisés
    CRegisterFactory<CMesh> m_CustomMeshFactory;

    //! Crée un type de mesh personnalisé
    CMesh* createCustomMesh(const QString& typeName, const QString& itemName = "");

    //! Ajoute un mesh
    void appendMesh(CMesh* pMesh);

    //! Implémentation de CMesh::IMeshListener
    virtual void onUpdate(CMesh* pMesh);

    //! Implémentation de CMesh::IMeshListener
    virtual void onDelete(CMesh* pMesh);

    //! Implémentation de CMeshBuffer::IMeshListener
    virtual void onDelete(CMeshBuffer* pBuffer);


    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<IMeshManagerListener*> m_MeshManagerListeners;

    //! Notifie les listeners qu'un mesh a été ajouté
    void notifyUpdate(CMesh* pMesh);

    //! Notifie les listeners qu'un mesh a été supprimé
    void notifyDelete(CMesh* pMesh);

    //! Liste des listeners
    QSet<IMeshBufferListener*> m_MeshBufferListeners;

    //! Notifie les listeners qu'un buffer a été supprimé
    void notifyDelete(CMeshBuffer* pBuffer);


private:

    //! Constructeur
    CMeshManager();

    //! Destructeur
    virtual ~CMeshManager();

    //! Conteneur des meshs
    QMap<QString, CMesh*> m_MeshByName;

    //! Conteneur des meshs
    QMap<int, CMesh*> m_MeshByID;

    static CMeshManager* s_pInstance;
    static QMutex s_Mutex;

    friend QDataStream& operator << (QDataStream& out,	const CMesh& mesh);
    friend QDataStream& operator >> (QDataStream& in,	CMesh& mesh);

    friend QDataStream& operator << (QDataStream& out,  const CSubMesh& mesh);
    friend QDataStream& operator >> (QDataStream& in,	CSubMesh& mesh);

    friend QDataStream& operator << (QDataStream& out,	const CMeshBuffer& buffer);
    friend QDataStream& operator >> (QDataStream& in,	CMeshBuffer& buffer);

    friend QDataStream& operator << (QDataStream& out,  const CBone& bone);
    friend QDataStream& operator >> (QDataStream& in,	CBone& bone);

    friend QDataStream& operator << (QDataStream& out,	const CVertexWeight& vw);
    friend QDataStream& operator >> (QDataStream& in,	CVertexWeight& vw);

};

//-----------------------------------------------------------------------------------------
template<typename CustomMesh> void CMeshManager::registerCustomMesh(const QString& typeName)
{
    m_CustomMeshFactory.registerClass<CustomMesh>(typeName);
}

//-----------------------------------------------------------------------------------------
template<typename CustomMesh> CustomMesh* CMeshManager::createCustomMesh(const QString& typeName, const QString& itemName /*= ""*/)
{
    m_CustomMeshFactory.registerClass<CustomMesh>(typeName);
    return dynamic_cast<CustomMesh*>(createCustomMesh(typeName, itemName));
}

//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CMesh& mesh);
QDataStream& operator >> (QDataStream& in,	CMesh& mesh);

QDataStream& operator << (QDataStream& out, const CMeshGroup& mesh);
QDataStream& operator >> (QDataStream& in,	CMeshGroup& mesh);

QDataStream& operator << (QDataStream& out, const CSubMesh& mesh);
QDataStream& operator >> (QDataStream& in,	CSubMesh& mesh);

QDataStream& operator << (QDataStream& out,	const CMeshBuffer& buffer);
QDataStream& operator >> (QDataStream& in,	CMeshBuffer& buffer);


QDataStream& operator << (QDataStream& out, const CBone& bone);
QDataStream& operator >> (QDataStream& in,	CBone& bone);

QDataStream& operator << (QDataStream& out,	const CVertexWeight& vw);
QDataStream& operator >> (QDataStream& in,	CVertexWeight& vw);

#endif // CMESHMANAGER_H
