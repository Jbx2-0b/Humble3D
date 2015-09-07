#include "CCloudsMesh.h"

//-----------------------------------------------------------------------------------------
CCloudsMesh::CCloudsMesh()
: CMesh("CloudsMesh")
, m_fProbabilityOfExtinction(fDefaultProbabilityOfExtinction)
{
	
}

//-----------------------------------------------------------------------------------------
void CCloudsMesh::update(int iSizeX, int iSizeY, int iSizeZ)
{
	m_Humidity.resize(iSizeX, iSizeY, iSizeZ);
	m_Clouds.resize(iSizeX, iSizeY, iSizeZ);
	m_Activation.resize(iSizeX, iSizeY, iSizeZ);
}

//-----------------------------------------------------------------------------------------
CCloudsMesh::~CCloudsMesh()
{

}
