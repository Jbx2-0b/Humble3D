#include "Shapes/CCoordinateSystem.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------------------------
CCoordinateSystem::CCoordinateSystem()
: CMesh("CoordinateSystem")
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------------------------
CCoordinateSystem::~CCoordinateSystem()
{
}

//-----------------------------------------------------------------------------------------
void CCoordinateSystem::updateGeometry()
{
    clearSubMeshs();

    CSubMesh* pArrowX = createSubMesh();
    initArrow(pArrowX, Color::eRed, eArrowAxisX);

    CSubMesh* pArrowY = createSubMesh();
    initArrow(pArrowY, Color::eGreen, eArrowAxisY);

    CSubMesh* pArrowZ = createSubMesh();
    initArrow(pArrowZ, Color::eBlue, eArrowAxisZ);
}

//-----------------------------------------------------------------------------------------
void CCoordinateSystem::initArrow(CSubMesh* pArrow, const Color& color, EnumArrowAxis eArrowAxis)
{
	pArrow->setPrimitiveType(ePrimitiveLine);

	CBuffer<QVector3D>& posBuffer = pArrow->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = pArrow->indicesBuffer();
   
	posBuffer << QVector3D(0., 0., 0.);

	switch (eArrowAxis)
	{

	case eArrowAxisX:
		posBuffer << QVector3D(1.f, 0.f, 0.f);
		posBuffer << QVector3D(0.8f, 0.2f, 0.f);
		posBuffer << QVector3D(0.8f, -0.2f, 0.f);
		break;

	case eArrowAxisY:
		posBuffer << QVector3D(0.f, 1.f, 0.f);
		posBuffer << QVector3D(0.2f, 0.8f, 0.f);
		posBuffer << QVector3D(-0.2f, 0.8f, 0.f);
		break;

	case eArrowAxisZ:
		posBuffer << QVector3D(0.0f,  0.0f, 1.0f);
		posBuffer << QVector3D(0.0f,  0.2f, 0.8f);
		posBuffer << QVector3D(0.0f, -0.2f, 0.8f);
		break;
	}

	idBuffer << 0 << 1 << 1 << 2 << 1 << 3;

    pArrow->setMaterialName(CMaterialManager::getInstance()->getMaterialNameByColor(color));
}
