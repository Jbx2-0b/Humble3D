#ifndef CCLOUDSMESH_H
#define CCLOUDSMESH_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"
#include "CArray3D.h"

static const real fDefaultProbabilityOfExtinction = 0.01f;

class LIB_ASSETS_SHARED_EXPORT CCloudsMesh : public CMesh
{
public:

	//! Constructeur
	CCloudsMesh();

	//! Destructeur
	virtual ~CCloudsMesh();

	void update(int iSizeX, int iSizeY, int iSizeZ);

private:
	
	inline bool act(int i, int j, int k) const { return m_Activation.get(i, j, k); }
	inline bool hum(int i, int j, int k) const { return m_Clouds.get(i, j, k); }
	inline bool cld(int i, int j, int k) const { return m_Activation.get(i, j, k); }

	inline bool nextHum(int i, int j, int k)
	{
		return hum(i, j, k) && !act(i, j, k);
	}

	inline bool nextCld(int i, int j, int k)
	{
		return cld(i, j, k) || act(i, j, k);
	}

	inline bool nextAct(int i, int j, int k)
	{
		return !act(i, j, k) && hum(i, j, k) && fAct(i, j, k);
	}

	inline bool fAct(int i, int j, int k)
	{
		return 
			act(i+1, j, k) || act(i, j+1, k) || act(i, j, k+1) || 
			act(i-1, j, k) || act(i, j-1, k) || act(i, j, k-1) || 
			act(i-2, j, k) || act(i+2, j, k) || act(i, j-2, k) || 
			act(i, j+2, k) || act(i, j, k-2);
	}

	CArray3D<bool> m_Humidity;
	CArray3D<bool> m_Clouds;
	CArray3D<bool> m_Activation;

    real m_fProbabilityOfExtinction;
};

#endif // CCLOUDSMESH_H
