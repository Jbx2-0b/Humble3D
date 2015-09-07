#ifndef CTEXTUREMANAGER_H
#define CTEXTUREMANAGER_H


// Lib
#include "lib_core_global.h"

// Foundations
#include "CTexture2D.h"
#include "CTexture3D.h"
#include "CTextureTarget.h"
#include "CTextureCube.h"
#include "CGeometryGlobal.h"

// Qt
#include <QMutex>
#include <QMutexLocker>

class LIB_CORE_SHARED_EXPORT CTextureManager : public ATexture::ITextureListener
{
public:

	//! Accesseur a l'instance de la classe.
	static CTextureManager* getInstance()
	{
		if (s_pInstance == 0)
		{
			QMutexLocker locker(&s_Mutex);

			if (s_pInstance == 0)
			{
				s_pInstance = new CTextureManager();
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

    class ITextureManagerListener
	{
	public:

		//! Pr�vient de l'ajout ou de la mise � jour d'un texture
		virtual void onUpdateTexture(ATexture* pTexture) = 0;

		//! Pr�vient de la suppression d'un texture
		virtual void onDeleteTexture(ATexture* pTexture) = 0;
	};

	//! Enregistre un listener
    void registerListener(ITextureManagerListener* pListener);

	//! Lib�re un listener
    void unregisterListener(ITextureManagerListener* pListener);

	//! Retourne le nombre de textures
    int getTextureCount() const												{ return m_TextureByName.size(); }

	//! Retourne la collection de textures
    const QList<ATexture*> getTextures() const								{ return m_TextureByName.values(); }

	//! Retourne la collection des de textures disponible
    const QList<QString> getTexturesNames() const							{ return m_TextureByName.keys(); }

	//! Retourne une texture en fonction de son nom
	ATexture* getTextureByName(const QString& name) const;

	//! Retourne une texture en fonction de son nom
	ATexture* getTextureByID(int iID) const;

	//! Retourne true si la texture existe
	bool isTextureExist(const QString& name) const;

	//! Retourne true si la texture existe
	bool isTextureExist(int iID) const;

    //! Cr�e une texture 2D non initialis�e
	CTexture2D* createTexture2D(const QString& name);

    //! Cr�e une texture 2D vide
	CTexture2D* createTexture2D(const QString& name, const QSize& size);

	//! Cr�e une nouvelle texture et retourne son pointeur
	CTexture2D* createTexture2D(const QString& name, const QString& textureFileName);

    //! Cr�e une texture cube
	CTextureCube* createTextureCube(const QString& name, const QStringList& textureFileNames);

    //! Cr�e une texture cube
    CTextureCube* createTextureCube(
            const QString& name,
            const QString& fileNamePosX,
            const QString& fileNameNegX,
            const QString& fileNamePosY,
            const QString& fileNameNegY,
            const QString& fileNamePosZ,
            const QString& fileNameNegZ);

    //! Cr�e une nouvelle texture de rendu
	CTextureTarget* createTextureTarget(const QString& name, const QSize& size, EnumInternalFormat eFormat);
	
    //! Supprime une texture
	void removeTexture(ATexture* pTexture);	

	//! Lib�re l'ensemble des textures du manager
    void clearTextures();

	//! Retourne un ID d'unit� de texture non utilis�
	int getUnReservedTextureUnit();

protected:

    //! Impl�mentation de ATexture::ITextureListener
	virtual void onUpdate(ATexture* pTexture);	

    //! Impl�mentation de ATexture::ITextureListener
	virtual void onDelete(ATexture* pTexture);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<ITextureManagerListener*> m_TextureManagerListeners;

    //! Notifie les listeners qu'un texture a �t� ajout�
    void notifyUpdate(ATexture* pTexture);

    //! Notifie les listeners qu'un texture a �t� supprim�
    void notifyDelete(ATexture* pTexture);

private:

	//! Constructeur
	CTextureManager();

	//! Destructeur
	virtual ~CTextureManager();

	//! Conteneur des textures
    QMap<QString, ATexture*> m_TextureByName;

	//! Conteneur des textures
    QMap<int, ATexture*> m_TextureByID;

	static CTextureManager* s_pInstance;
	static QMutex s_Mutex;
};

#endif // CTEXTUREMANAGER_H
