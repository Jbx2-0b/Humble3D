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

		//! Pr�vient de l'ajout ou de la mise � jour d'un mat�riau
		virtual void onUpdate(CMaterial* pMaterial) = 0;

		//! Pr�vient de la suppression d'un mat�riau
		virtual void onDelete(CMaterial* pMaterial) = 0;
	};

	//! Enregistre un listener
    void registerListener(IMaterialManagerListener* pListener);

	//! Lib�re un listener
    void unregisterListener(IMaterialManagerListener* pListener);

	//! Retourne le nombre de mat�riaux
	int getMaterialCount() const											{ return m_Materials.size(); }

	//! Retourne la collection de mat�riaux
    const QList<CMaterial*> getMaterials() const							{ return m_Materials.values(); }

    //! Retourne la collection des noms des mat�riaux disponibles
	const QList<QString> getMaterialsNames() const							{ return m_Materials.keys(); }

	//! Retourne un materiau en fonction de son nom
	CMaterial* getMaterialByName(const QString& name) const;

	//! Retourne un materiau en fonction de son nom
	CMaterial* getMaterialByID(int iID) const;

	//! Retourne true si le mat�riau existe
	bool isMaterialExist(const QString& name) const;

	//! Retourne true si le mat�riau existe
	bool isMaterialExist(int iID) const;

	//! Cr�e un nouveau mat�riau et retourne son pointeur
	CMaterial* createMaterial(const QString& name = "Material");

    //! Supprime un materiau
	void removeMaterial(CMaterial* pMaterial);	

	//! Lib�re l'ensemble des mat�riaux du manager
    void clearMaterials();

    //! Initialise les mat�raux de couleurs
    void initColorMaterials();

    //! Retourne un mat�riaux en fonction d'une couleur
    QString getMaterialNameByColor(const Color& color);

protected:

    //! Impl�mentation de CMaterial::IMaterialListener
	virtual void onUpdate(CMaterial* pMaterial);	

    //! Impl�mentation de CMaterial::IMaterialListener
	virtual void onDelete(CMaterial* pMaterial);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<IMaterialManagerListener*> m_MaterialManagerListeners;

    //! Notifie les listeners qu'un mat�riau a �t� ajout�
    void notifyUpdate(CMaterial* pMaterial);

    //! Notifie les listeners qu'un mat�riau a �t� supprim�
    void notifyDelete(CMaterial* pMaterial);

private:

	//! Constructeur
	CMaterialManager();

	//! Destructeur
	virtual ~CMaterialManager();

	//! Conteneur des mat�riaux
	QMap<QString, CMaterial*> m_Materials;

	//! Conteneur des mat�riaux
	QMap<int, CMaterial*> m_MaterialIDs;

	static CMaterialManager* s_pInstance;
	static QMutex s_Mutex;

};

#endif // CMATERIALMANAGER_H
