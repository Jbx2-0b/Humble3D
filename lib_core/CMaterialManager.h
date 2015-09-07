#ifndef CMATERIALMANAGER_H
#define CMATERIALMANAGER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMaterial.h"
#include "CGeometryGlobal.h"

// Qt
#include <QMutex>
#include <QMutexLocker>


class LIB_CORE_SHARED_EXPORT CMaterialManager : public CMaterial::IMaterialListener
{
public:

    //! Accesseur de l'instance de la classe.
    static CMaterialManager* getInstance()
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CMaterialManager();
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

    class IMaterialManagerListener
    {
    public:

        //! Prévient de l'ajout ou de la mise à jour d'un matériau
        virtual void onUpdate(CMaterial* pMaterial) = 0;

        //! Prévient de la suppression d'un matériau
        virtual void onDelete(CMaterial* pMaterial) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMaterialManagerListener* pListener);

    //! Libére un listener
    void unregisterListener(IMaterialManagerListener* pListener);

    //! Retourne le nombre de matériaux
    int getMaterialCount() const											{ return m_Materials.size(); }

    //! Retourne la collection de matériaux
    const QList<CMaterial*> getMaterials() const							{ return m_Materials.values(); }

    //! Retourne la collection des noms des matériaux disponibles
    const QList<QString> getMaterialsNames() const							{ return m_Materials.keys(); }

    //! Retourne un materiau en fonction de son nom
    CMaterial* getMaterialByName(const QString& name) const;

    //! Retourne un materiau en fonction de son nom
    CMaterial* getMaterialByID(int iID) const;

    //! Retourne true si le matériau existe
    bool isMaterialExist(const QString& name) const;

    //! Retourne true si le matériau existe
    bool isMaterialExist(int iID) const;

    //! Crée un nouveau matériau et retourne son pointeur
    CMaterial* createMaterial(const QString& name = "Material");

    //! Supprime un materiau
    void removeMaterial(CMaterial* pMaterial);

    //! Libére l'ensemble des matériaux du manager
    void clearMaterials();

    //! Initialise les matéraux de couleurs
    void initColorMaterials();

    //! Retourne un matériaux en fonction d'une couleur
    QString getMaterialNameByColor(const Color& color);

protected:

    //! Implémentation de CMaterial::IMaterialListener
    virtual void onUpdate(CMaterial* pMaterial);

    //! Implémentation de CMaterial::IMaterialListener
    virtual void onDelete(CMaterial* pMaterial);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<IMaterialManagerListener*> m_MaterialManagerListeners;

    //! Notifie les listeners qu'un matériau a été ajouté
    void notifyUpdate(CMaterial* pMaterial);

    //! Notifie les listeners qu'un matériau a été supprimé
    void notifyDelete(CMaterial* pMaterial);

private:

    //! Constructeur
    CMaterialManager();

    //! Destructeur
    virtual ~CMaterialManager();

    //! Conteneur des matériaux
    QMap<QString, CMaterial*> m_Materials;

    //! Conteneur des matériaux
    QMap<int, CMaterial*> m_MaterialIDs;

    static CMaterialManager* s_pInstance;
    static QMutex s_Mutex;

};

#endif // CMATERIALMANAGER_H
