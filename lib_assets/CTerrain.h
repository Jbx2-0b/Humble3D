#ifndef CTERRAIN_H
#define CTERRAIN_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"
#include "CMapDictionary.h"


class CNormalComputer;

class LIB_ASSETS_SHARED_EXPORT CTerrain : public CMesh
{

public:
	//! Constructeur
	CTerrain();

	//! Destructeur
	virtual ~CTerrain();

	//! Initialise le terrain
	void update(unsigned int iResolution, unsigned int iTextureResolution, const QVector3D& size, bool bComputeNormals = true);

	//! Définit les altitudes. Les valeurs doivent être normalisées entre [0; 1]
	void setHeights(const CMapKey& key, CMapDictionary* pMapDictionary);

	//! Retourne la taille du terrain
	QVector3D getSize() const { return m_Size; }

protected:

	virtual void updateGeometry() {}

private:

	static QMap<int, CNormalComputer> s_NormalComputers;

	inline QVector3D getStartingPosition(int i, int j) const;

	CMeshBuffer* m_pBuffer;

	static int s_iMaterialID;

	CMaterial* m_pMaterial;

	int m_iResolution;

	int m_iTextureResolution;

	QVector3D m_Size;
};


class CNormalComputer
{
public:
	CNormalComputer();

	CNormalComputer(int iResolution, int iLOD);

	void compute(const CMapKey& key, CMapDictionary* pMapDictionary, CBuffer<QVector3D>& nBuffer);

private:
	void setNormalFace1(int l, int w, const QVector3D& v)	{ normalFace1[l + w * (m_iResolution + 1)] = v; }
	void setNormalFace2(int l, int w, const QVector3D& v)	{ normalFace2[l + w * (m_iResolution + 1)] = v; }

	QVector3D getNormalFace1(int l, int w) const			{ return normalFace1[l + w * (m_iResolution + 1)]; }
	QVector3D getNormalFace2(int l, int w) const			{ return normalFace2[l + w * (m_iResolution + 1)]; }

	int m_iResolution;
	int m_iLOD;

	QVector<QVector3D> normalFace1;
	QVector<QVector3D> normalFace2;
};

#endif // CTERRAIN_H
