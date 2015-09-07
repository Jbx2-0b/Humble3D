#ifndef CMESHGEOMETRY_H
#define CMESHGEOMETRY_H

// Lib
#include "lib_geometry_global.h"

// Fundations
#include "ALockable.h"
#include "AWashable.h"
#include "CMeshBuffer.h"

// Qt
#include <QThread>

class LIB_GEOMETRYSHARED_EXPORT CMeshGeometry 
	: public ALockable
	, public AWashable
{
public:

	//! Constructeur
	CMeshGeometry();

	//! Destructeur
	~CMeshGeometry();

	//! Accesseur les buffers de vertex
	const QList<CMeshBuffer*>& meshBuffers() const { return m_MeshBuffers; }

	//! Accesseur les buffers de vertex
	QList<CMeshBuffer*>& meshBuffers() { return m_MeshBuffers; }

	//--------------------------------------------------------------------------------------------
	// Gestions des mesh buffers
	//--------------------------------------------------------------------------------------------

	//! Crée et ajoute un buffer de vertex
	CMeshBuffer* createMeshBuffer();

	//! Retire un buffer de vertex
	void removeMeshBuffer(CMeshBuffer* pBuffer);

	//! Supprime l'ensemble des buffers de vertex
	void clearMeshBuffers();

	//! Retourne la boîte englobant l'objet
	const CBox3D& getBoundingBox();

	//! Met à jour la géométrie
	void update(bool bMultithread);

protected:

	void computeBoundingBox();

	//! Boîte englobante
	CBox3D m_BoundingBox;

	//! Vrai si la maj de la boîte englobante nécessaire
	bool m_bNeedBoundingBoxUpdate;

	//! Buffers de vertex
	QList<CMeshBuffer*> m_MeshBuffers;

private:

	/*
	//! Transmet le message de mise à jour
	virtual void notifyUpdate();

	//! Transmet le message de suppression
	virtual void notifyDelete();

	//! Liste des listeners
	QList<CMeshGeometry::IListener*> m_Listeners;*/

	//! Fonction appelée à l'initialisation de l'objet
	virtual void updateGeometry() {}

	//! Instancieur de geométrie
	class LIB_GEOMETRYSHARED_EXPORT CGeometryInstancer : public QThread
	{
	public:
		CGeometryInstancer(QObject *parent = 0);

		virtual ~CGeometryInstancer();

		void addMeshGeometry(CMeshGeometry* pMesh);

	protected:

		virtual void run();

		CMeshGeometry* getAvailableMeshGeometry();

		QList<CMeshGeometry*> m_MeshGeometryList;

		static const int s_iWaitTime = 10;

		mutable QMutex m_Mutex;

	};

	static CGeometryInstancer s_GeometryInstancer;
};

#endif // CMESHGEOMETRY_H
