#include "CArrow.h"
#include "CSceneManager.h"
#include "CMath.h"

//-----------------------------------------------------------------------------------------
CArrow::CArrow()
    : CMesh("CArrow")
    , m_Origin(0., 0., 0.)
    , m_Direction(1., 1., 1.)
{
}

//-----------------------------------------------------------------------------------------
void CArrow::setDirection(const QVector3D& direction)
{
    m_Direction = direction;
    update();
}

//-----------------------------------------------------------------------------------------
CArrow::~CArrow()
{
}

//-----------------------------------------------------------------------------------------
void CArrow::updateGeometry()
{
    CSubMesh* pArrow = createSubMesh();

    pArrow->setPrimitiveType(ePrimitiveLine);

    CBuffer<QVector3D>& posBuffer = pArrow->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = pArrow->indicesBuffer();

    posBuffer << QVector3D(0., 0., 0.);

    real angle = Math::computeRadAngle(QVector3D(1., 0., 0.), m_Direction.normalized());
    QVector3D normal = QVector3D::normal(QVector3D(1., 0., 0.), m_Direction.normalized());
    QQuaternion q = QQuaternion::fromAxisAndAngle(normal, Math::radToDeg(angle));

    real length = m_Direction.length();

    posBuffer << m_Origin + QVector3D(1.f, 0.f, 0.f) * length;
    posBuffer << m_Origin + QVector3D(0.8f, +0.2f, 0.f) * length;
    posBuffer << m_Origin + QVector3D(0.8f, -0.2f, 0.f) * length;

    for (int i = 0; i < posBuffer.count(); ++i)
    {
        posBuffer[i] = q.rotatedVector(posBuffer[i]);
    }

    idBuffer << 0 << 1 << 1 << 2 << 1 << 3;

    pArrow->setMaterialName(CMaterialManager::getInstance().getMaterialNameByColor(Color::eWhite));
}

