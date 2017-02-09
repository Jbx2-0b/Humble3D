#include "CMeshManager.h"
#include "CLogManager.h"
#include "CBinaryMesh.h"
#include "CBinaryLoader.h"

//-----------------------------------------------------------------------------------------
CMeshManager::CMeshManager()
{

}

//-----------------------------------------------------------------------------------------
CMeshManager &CMeshManager::getInstance()
{
    static CMeshManager instance;
    return instance;
}

//-----------------------------------------------------------------------------------------
CMeshManager::~CMeshManager()
{
    clearMeshs();
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CMeshManager::registerListener(IMeshManagerListener* pListener)
{
    m_MeshManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::unregisterListener(IMeshManagerListener* pListener)
{
    m_MeshManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::registerMeshBufferListener(IMeshBufferListener* pListener)
{
    m_MeshBufferListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::unregisterMeshBufferListener(IMeshBufferListener* pListener)
{
    m_MeshBufferListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::onUpdate(CMesh* pMesh)
{
    notifyUpdate(pMesh);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::onDelete(CMesh* /*pMesh*/)
{
    // Nothing to do
}

//-----------------------------------------------------------------------------------------
CMesh* CMeshManager::getMeshByName(const QString& name) const
{
    if (m_MeshByName.contains(name))
    {
        return m_MeshByName.value(name);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CMesh* CMeshManager::getMeshByID(int iID) const
{
    if (m_MeshByID.contains(iID))
    {
        return m_MeshByID.value(iID);
    }
    return 0;
}

//-----------------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const QString& name)
{
    CMesh* pMesh = new CMesh(name);
    appendMesh(pMesh);
    //LogManager.addMessage(eINFO, "CMeshManager: Create mesh " + pMesh->getName());
    return pMesh;
}

//-----------------------------------------------------------------------------------------
CMesh* CMeshManager::loadMesh(const QString& fileName)
{
    CMesh* pMesh = new CBinaryMesh(fileName, QFileInfo(fileName).baseName());
    appendMesh(pMesh);
    //LogManager.addMessage(eINFO, "CMeshManager: Load mesh " + fileName);
    return pMesh;
}

//-----------------------------------------------------------------------------------------
bool CMeshManager::saveMesh(CMesh* pMesh, const QString& fileName, int iCompressionFactor)
{
    //LogManager.addMessage(eINFO, "CMeshManager::saveMesh() : " + fileName);

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QByteArray uncompressData;
        QDataStream out(&uncompressData, QIODevice::WriteOnly);

        int iSubMeshCount = pMesh->subMeshs().count();
        out << iSubMeshCount;

        for (CSubMesh* pSubMesh : pMesh->subMeshs())
        {
            out << *pSubMesh;
        }

        QByteArray compressData = qCompress(uncompressData, iCompressionFactor);
        file.write(compressData, compressData.size());
        file.close();

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------
CMeshGroup CMeshManager::loadMeshGroup(const QString& fileName)
{
    CMeshGroup meshGroup;
    CBinaryLoader::load(fileName, &meshGroup);

    for (CMesh* pMesh : meshGroup)
    {
        appendMesh(pMesh);
    }
    LogManager.addMessage(eINFO, "CMeshManager: Load mesh group " + fileName);
    return meshGroup;
}

//-----------------------------------------------------------------------------------------
bool CMeshManager::saveMeshGroup(const CMeshGroup& meshGroup, const QString& fileName, int iCompressionFactor)
{
    if (CBinaryLoader::save(fileName, &meshGroup, iCompressionFactor))
    {
        LogManager.addMessage(eINFO, "CMeshManager: Save mesh group " + fileName);
        return true;
    }
    LogManager.addMessage(eINFO, "CMeshManager: Error saving mesh group " + fileName);
    return false;
}

//-----------------------------------------------------------------------------------------
CMesh* CMeshManager::createCustomMesh(const QString& typeName, const QString& itemName /*= ""*/)
{
    if (CMesh* pCustomMesh = m_CustomMeshFactory.create(typeName))
    {
        pCustomMesh->setName(itemName);
        appendMesh(pCustomMesh);
        //LogManager.addMessage(eINFO, "CMeshManager: Create mesh " + pCustomMesh->getName());
        return pCustomMesh;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------
void CMeshManager::appendMesh(CMesh* pMesh)
{
    pMesh->lock();
    m_MeshByName[pMesh->getName()] = pMesh;
    m_MeshByID[pMesh->getID()] = pMesh;
    pMesh->registerListener(this);
    pMesh->update();
    notifyUpdate(pMesh);
    pMesh->unlock();
}

//-----------------------------------------------------------------------------------------
void CMeshManager::removeMesh(CMesh* pMesh)
{
    pMesh->unregisterListener(this);

    for (CSubMesh* pSubMesh : pMesh->subMeshs())
    {
        notifyDelete(pSubMesh);
    }

    //notifyDelete(pMesh);
    m_MeshByName.remove(pMesh->getName());
    m_MeshByID.remove(pMesh->getID());
    delete pMesh;
}

//-----------------------------------------------------------------------------------------
void CMeshManager::clearMeshs()
{
    for (CMesh* pMesh : m_MeshByID)
    {
        pMesh->unregisterListener(this);

        for (CSubMesh* pSubMesh : pMesh->subMeshs())
        {
            notifyDelete(pSubMesh);
        }

        notifyDelete(pMesh);
        delete pMesh;
    }

    m_MeshByName.clear();
    m_MeshByID.clear();
}

//-----------------------------------------------------------------------------------------
bool CMeshManager::isMeshExist(const QString& name) const
{
    return m_MeshByName.contains(name);
}

//-----------------------------------------------------------------------------------------
bool CMeshManager::isMeshExist(int iID) const
{
    return m_MeshByID.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::notifyUpdate(CMesh* pMesh)
{
    for (IMeshManagerListener* pListener : m_MeshManagerListeners)
    {
        pListener->onUpdate(pMesh);
    }
}

//-----------------------------------------------------------------------------------------
void CMeshManager::notifyDelete(CMesh* pMesh)
{
    for (IMeshManagerListener* pListener : m_MeshManagerListeners)
    {
        pListener->onDelete(pMesh);
    }
}

//-----------------------------------------------------------------------------------------
// CMeshBuffer::IMeshBufferListener
//-----------------------------------------------------------------------------------------
void CMeshManager::onDelete(CMeshBuffer* pBuffer)
{
    notifyDelete(pBuffer);
}

//-----------------------------------------------------------------------------------------
void CMeshManager::notifyDelete(CMeshBuffer* pBuffer)
{
    for (IMeshBufferListener* pListener : m_MeshBufferListeners)
    {
        pListener->onDeleteMeshBuffer(pBuffer);
    }
}

//-----------------------------------------------------------------------------------------
// CMeshGroup
//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CMeshGroup& meshGroup)
{
    out << meshGroup.count();

    for (CMesh* pMesh : meshGroup)
    {
        out << *pMesh;
    }

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in, CMeshGroup& meshGroup)
{
    unsigned int iMeshCount;

    in >> iMeshCount;

    for (unsigned int i = 0; i < iMeshCount; ++i)
    {
        CMesh* pMesh = CMeshManager::getInstance().createMesh();
        in >> *pMesh;
        meshGroup.append(pMesh);
    }

    return in;
}

//-----------------------------------------------------------------------------------------
// CMesh
//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CMesh& mesh)
{
    out << mesh.subMeshs().count();
    for (CSubMesh* pSubMesh : mesh.subMeshs())
    {
        out << *pSubMesh;
    }

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in, CMesh& mesh)
{
    unsigned int iSubMeshCount;

    in >> iSubMeshCount;

    for (unsigned int i = 0; i < iSubMeshCount; ++i)
    {
        CSubMesh* pSubMesh = mesh.createSubMesh();
        in >> *pSubMesh;
    }

    mesh.computeNormals();
    mesh.computeTangents();

    return in;
}

//-----------------------------------------------------------------------------------------
// CSubMesh
//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CSubMesh& mesh)
{
    out << *dynamic_cast<const CMeshBuffer*>(&mesh);
    out << mesh.getMaterialName();

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in, CSubMesh& mesh)
{
    in >> *dynamic_cast<CMeshBuffer*>(&mesh);

    QString materialName;
    in >> materialName;

    mesh.setMaterialName(materialName);

    return in;
}

//-----------------------------------------------------------------------------------------
// CMeshBuffer
//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out,	const CMeshBuffer& buffer)
{
    out << (int) buffer.getPrimitiveType();
    out << buffer.getPrimitiveIndex();
    out << buffer.positionsBuffer();
    out << buffer.indicesBuffer();

    for (unsigned int i = 0; i < DefaultMaxTextureChannelCount; ++i)
    {
        out << buffer.texCoordBuffer(i);
    }

    out << buffer.getAutoComputeNormals();

    if (!buffer.getAutoComputeNormals())
    {
        out << buffer.normalsBuffer();
    }

    out << buffer.bonesBuffer();

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in,	CMeshBuffer& buffer)
{
    int iPrimitiveType = 0;
    in >> iPrimitiveType;
    buffer.setPrimitiveType((EnumPrimitiveType)iPrimitiveType);

    unsigned int uiFaceEdgeCount = 0;
    in >> uiFaceEdgeCount;
    buffer.setPrimitiveIndex(uiFaceEdgeCount);

    CBuffer<QVector3D>& vertexBuffer = buffer.positionsBuffer();
    in >> vertexBuffer;

    CBuffer<IndiceType>& indicesBuffer = buffer.indicesBuffer();
    in >> indicesBuffer;

    for (unsigned int i = 0; i < DefaultMaxTextureChannelCount; ++i)
    {
        CBuffer<QVector3D>& texCoordBuffer = buffer.texCoordBuffer(i);
        in >> texCoordBuffer;
    }

    bool bAutoComputeNormals;
    in >> bAutoComputeNormals;
    buffer.setAutoComputeNormals(bAutoComputeNormals);

    if (!bAutoComputeNormals)
    {
        CBuffer<QVector3D>& normalsBuffer = buffer.normalsBuffer();
        in >> normalsBuffer;
    }

    CBuffer<CBone>& bonesBuffer = buffer.bonesBuffer();
    in >> bonesBuffer;

    if (buffer.hasSkeleton())
    {
        buffer.computeBones();
    }

    return in;
}


//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CBone& bone)
{
    out << bone.getNodeName();
    out << bone.getOffsetMatrix();
    out << bone.vertexWeights();

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in,	CBone& bone)
{
    QString nodeName;
    in >> nodeName;
    bone.setNodeName(nodeName);

    QMatrix4x4 mOffset;
    in >> mOffset;
    bone.setOffsetMatrix(mOffset);

    in >> bone.vertexWeights();

    return in;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out,	const CVertexWeight& vw)
{
    out << vw.getVertexID();
    out << vw.getWeight();

    return out;
}

//-----------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in,	CVertexWeight& vw)
{
    int iVertexID;
    in >> iVertexID;
    vw.setVertexID(iVertexID);

    double dWeight;
    in >> dWeight;
    vw.setWeight(dWeight);

    return in;
}

