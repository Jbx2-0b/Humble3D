#ifndef CBILLBOARD_H
#define CBILLBOARD_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "ARenderableItem.h"
#include "CMeshBuffer.h"

class CSceneManager;

class LIB_CORE_SHARED_EXPORT CBillboard : public ARenderableItem
{

public:

	//! Constructeur
    CBillboard(CSceneManager* pSceneManager, const QString& name);

	//! Destructeur
    virtual ~CBillboard();

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Billboard"; }

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox(void) const { return m_MeshBuffer.getBoundingBox(); }

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const { return m_MeshBuffer.getPolygonCount(); }

    //! Retourne vrai si peut être rendu
    virtual bool isRenderable() const { return !m_MeshBuffer.positionsBuffer().isEmpty(); }

	void addPosition(const QVector3D& position);

    void addPositions(const QVector<QVector3D>& positions);

    void clearPositions();

protected:

    //-------------------------------------------------------------------------------------------------
    // Implémentation ARenderableItem
    //-------------------------------------------------------------------------------------------------

    //! Effectue le rendu
    virtual void render(ARenderer* pRenderer);

private:

    CMeshBuffer m_MeshBuffer;
	real m_fSize;
	QMatrix4x4 m_CurrentModelViewMatrix;
};

#endif // CBILLBOARD_H


