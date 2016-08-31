#include "CMeshBuffer.h"


//--------------------------------------------------------------------------
void CMeshBuffer::append(const CMeshBuffer& buffer)
{
    unsigned int iStartCount = m_PositionsBuffer.count();

    m_PositionsBuffer << buffer.positionsBuffer();

    foreach (unsigned int id, buffer.indicesBuffer())
    {
        m_IndicesBuffer << id + iStartCount;
    }

    m_NormalsBuffer << buffer.normalsBuffer();
    m_TangeantsBuffer << buffer.tangentsBuffer();
    m_BitangeantsBuffer << buffer.bitangentsBuffer();

    for (unsigned int i = 0; i < DefaultMaxTextureChannelCount; ++i)
    {
        m_TexCoordsBuffer[i] << buffer.texCoordBuffer(i);
    }

    m_FaceNormalsBuffer << buffer.faceNormalsBuffer();

    m_bNeedBoundingBoxUpdate = true;
    m_bDirty = true;
}

//--------------------------------------------------------------------------
void CMeshBuffer::clear()
{
    m_PositionsBuffer.clear();
    m_IndicesBuffer.clear();
    m_NormalsBuffer.clear();
    m_TangeantsBuffer.clear();
    m_BitangeantsBuffer.clear();
    m_BonesBuffer.clear();

    for (unsigned int i = 0; i < DefaultMaxTextureChannelCount; ++i)
    {
        m_TexCoordsBuffer[i].clear();
    }

    m_FaceNormalsBuffer.clear();
    m_bDirty = true;
}

//--------------------------------------------------------------------------
CUserBuffer* CMeshBuffer::createUserBuffer(const QString& name, int iTupleSize)
{ 
    m_UserBuffers.insert(name, CUserBuffer());
    m_UserBuffers[name].setTupleSize(iTupleSize);

    return &m_UserBuffers[name];
}

//--------------------------------------------------------------------------
void CMeshBuffer::wash() const
{ 
    m_bDirty = false;
    m_PositionsBuffer.wash();
    m_IndicesBuffer.wash();
    m_NormalsBuffer.wash();
    m_BonesIDsBuffer.wash();
    m_BonesWeightsBuffer.wash();

    for (int i = 0; i < DefaultMaxTextureChannelCount; ++i)
    {
        m_TexCoordsBuffer[i].wash();
    }
}

//--------------------------------------------------------------------------
unsigned int CMeshBuffer::getPolygonCount() const
{
    switch (m_ePrimitiveType)
    {
    case ePrimitiveTriangleStrip:
        return m_IndicesBuffer.count() - 2;

    default:
        return m_IndicesBuffer.count() / m_uiPrimitiveIndex;
    }

    return 0; // avoid warning
}

//--------------------------------------------------------------------------
const CBox3D& CMeshBuffer::getBoundingBox() const
{
    if (m_bNeedBoundingBoxUpdate)
    {
        computeBoundinBox();
        m_bNeedBoundingBoxUpdate = false;
    }

    return m_BoundingBox;
}

//--------------------------------------------------------------------------
void CMeshBuffer::translate(real dX, real dY, real dZ)
{
    for (int i = 0; i < m_PositionsBuffer.count(); ++i)
    {
        m_PositionsBuffer[i] += QVector3D(dX, dY, dZ);
    }

    m_bDirty = true;
}

//--------------------------------------------------------------------------
void CMeshBuffer::scale(real dX, real dY, real dZ)
{
    QMatrix4x4 transformation;
    transformation.scale(dX, dY, dZ);

    for (int i = 0; i < m_PositionsBuffer.count(); ++i)
    {
        m_PositionsBuffer[i] = transformation * m_PositionsBuffer[i];
    }

    if (m_bAutoComputeNormals)
        computeNormals();

    m_bDirty = true;
}

//--------------------------------------------------------------------------
void CMeshBuffer::rotate(const QVector3D &vAxis, real dAngle)
{
    QMatrix4x4 transformation;
    transformation.rotate(QQuaternion::fromAxisAndAngle(vAxis, dAngle));

    for (int i = 0; i < m_PositionsBuffer.count(); ++i)
    {
        m_PositionsBuffer[i] = transformation * m_PositionsBuffer[i];
    }

    for (int i = 0; i < m_NormalsBuffer.count(); ++i)
    {
        m_NormalsBuffer[i] = transformation * m_NormalsBuffer[i];
    }

    for (int i = 0; i < m_TangeantsBuffer.count(); ++i)
    {
        m_TangeantsBuffer[i] = transformation * m_TangeantsBuffer[i];
    }

    for (int i = 0; i < m_BitangeantsBuffer.count(); ++i)
    {
        m_BitangeantsBuffer[i] = transformation * m_BitangeantsBuffer[i];
    }

    m_bNeedBoundingBoxUpdate = true;
    m_bDirty = true;
}

//--------------------------------------------------------------------------
void CMeshBuffer::computeNormals()
{
    m_NormalsBuffer = CBuffer<QVector3D>(m_PositionsBuffer.count());
    m_FaceNormalsBuffer = CBuffer<QVector3D>(getPolygonCount());

    switch (m_ePrimitiveType)
    {
    case ePrimitiveTriangleStrip:
    {
        for (unsigned int i = 0; i < getPolygonCount(); i++)
        {
            QVector3D AB = m_PositionsBuffer[m_IndicesBuffer[i + 1]] - m_PositionsBuffer[m_IndicesBuffer[i]];
            QVector3D AC = m_PositionsBuffer[m_IndicesBuffer[i + 2]] - m_PositionsBuffer[m_IndicesBuffer[i]];

            m_FaceNormalsBuffer[i] = QVector3D::normal(AB, AC);

            m_NormalsBuffer[m_IndicesBuffer[i + 0]] += m_FaceNormalsBuffer[i];
            m_NormalsBuffer[m_IndicesBuffer[i + 1]] += m_FaceNormalsBuffer[i];
            m_NormalsBuffer[m_IndicesBuffer[i + 2]] += m_FaceNormalsBuffer[i];

            i == 0 ? i+= 1 : 0;
        }
    }
        break;

    default:
    {
        for (unsigned int i = 0; i < getPolygonCount(); i++)
        {
            QVector3D AB = m_PositionsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex + 1]] - m_PositionsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex]];
            QVector3D AC = m_PositionsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex + 2]] - m_PositionsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex]];

            m_FaceNormalsBuffer[i] = QVector3D::normal(AB, AC);

            m_NormalsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex + 0]] += m_FaceNormalsBuffer[i];
            m_NormalsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex + 1]] += m_FaceNormalsBuffer[i];
            m_NormalsBuffer[m_IndicesBuffer[i * m_uiPrimitiveIndex + 2]] += m_FaceNormalsBuffer[i];
        }
    }
        break;
    }


    for (unsigned int i = 0; i < (unsigned int)m_PositionsBuffer.count(); i++)
    {
        m_NormalsBuffer[i].normalize();
    }
}


//--------------------------------------------------------------------------
void CMeshBuffer::computeTangents()
{
    // TODO
}

//--------------------------------------------------------------------------
void CMeshBuffer::computeBones()
{
    m_BonesIDsBuffer = CBuffer<TBoneIDs>(m_PositionsBuffer.count()); //, TBoneIDs());
    m_BonesWeightsBuffer = CBuffer<TBoneIDs>(m_PositionsBuffer.count()); //, TBoneIDs());

    for (int i = 0; i < m_BonesBuffer.count(); ++i)
    {
        const QList<CVertexWeight>& vertexWeights = m_BonesBuffer[i].vertexWeights();

        for (int j = 0; j < vertexWeights.count(); ++j)
        {
            

            int iVertexID = vertexWeights[j].getVertexID();
            real dBoneWeight = vertexWeights[j].getWeight();

            /*
            unsigned int iCurrentID = 0;
            bool bTreatBone = true;

            while (m_BonesWeightsBuffer[iVertexID][iCurrentID] != 0.)
            {
                iCurrentID++;

                if (iCurrentID > DefaultMaxBonesPerVertex)
                {
                    qDebug() << "CMeshBuffer::computeBones() : Bone per vertex > 4, non géré !";
                    bTreatBone = false;
                }
            }

            if (bTreatBone)
            {*/
            m_BonesIDsBuffer[iVertexID] = (float)i;
            m_BonesWeightsBuffer[iVertexID] = dBoneWeight;
            //}
        }
    }
}

//--------------------------------------------------------------------------
bool CMeshBuffer::intersection(const CRay& ray, real* dDistance, const QMatrix4x4& transformation /*= QMatrix4x4()*/) const
{
    bool bIntersect = false;

    int iIncr = 3;

    if (m_ePrimitiveType == ePrimitiveTriangleStrip)
        iIncr = 1;

    for (unsigned int iIndice = 0; (iIndice + 2) < (unsigned int) m_IndicesBuffer.count(); iIndice += iIncr)
    {
        CTriangle triangle(m_PositionsBuffer[m_IndicesBuffer[iIndice]], m_PositionsBuffer[m_IndicesBuffer[iIndice + 1]], m_PositionsBuffer[m_IndicesBuffer[iIndice + 2]]);
        triangle.transform(transformation);

        real dD = std::numeric_limits<real>::infinity();

        if (triangle.intersection(ray, &dD))
        {
            if (dD < *dDistance)
            {
                *dDistance = dD;
                bIntersect = true;
            }
        }
    }

    return bIntersect;
}

//--------------------------------------------------------------------------
bool CMeshBuffer::intersects(const CRay &ray, const QMatrix4x4& transformation /*= QMatrix4x4()*/) const
{
    real dDistance = 0.;
    return intersection(ray, &dDistance, transformation);
}

//--------------------------------------------------------------------------
CMeshBuffer& CMeshBuffer::operator<<(const CMeshBuffer& meshBuffer)
{
    append(meshBuffer);

    return *this;
}

//--------------------------------------------------------------------------
void CMeshBuffer::computeBoundinBox() const
{
    if (m_PositionsBuffer.isEmpty())
    {
        m_BoundingBox.setNull();
        return;
    }

    m_BoundingBox.getMinimum().setX(m_PositionsBuffer[0].x());
    m_BoundingBox.getMinimum().setY(m_PositionsBuffer[0].y());
    m_BoundingBox.getMinimum().setZ(m_PositionsBuffer[0].z());

    m_BoundingBox.getMaximum().setX(m_PositionsBuffer[0].x());
    m_BoundingBox.getMaximum().setY(m_PositionsBuffer[0].y());
    m_BoundingBox.getMaximum().setZ(m_PositionsBuffer[0].z());


    for (int i = 0; i < m_PositionsBuffer.count(); ++i)
    {
        if (m_PositionsBuffer[i].x() < m_BoundingBox.getMinimum().x())
            m_BoundingBox.getMinimum().setX(m_PositionsBuffer[i].x());

        if (m_PositionsBuffer[i].y() < m_BoundingBox.getMinimum().y())
            m_BoundingBox.getMinimum().setY(m_PositionsBuffer[i].y());

        if (m_PositionsBuffer[i].z() < m_BoundingBox.getMinimum().z())
            m_BoundingBox.getMinimum().setZ(m_PositionsBuffer[i].z());

        if (m_PositionsBuffer[i].x() > m_BoundingBox.getMaximum().x())
            m_BoundingBox.getMaximum().setX(m_PositionsBuffer[i].x());

        if (m_PositionsBuffer[i].y() > m_BoundingBox.getMaximum().y())
            m_BoundingBox.getMaximum().setY(m_PositionsBuffer[i].y());

        if (m_PositionsBuffer[i].z() > m_BoundingBox.getMaximum().z())
            m_BoundingBox.getMaximum().setZ(m_PositionsBuffer[i].z());
    }
}
