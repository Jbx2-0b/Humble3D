#include "CGLMeshBuffer.h"

GLenum CGLMeshBuffer::s_IndiceValuesType = GL_UNSIGNED_INT;

//--------------------------------------------------------------------------
void CGLMeshBuffer::setIndiceValuesType(GLenum type)
{
    s_IndiceValuesType = type;
}

//-------------------------------------------------------------------------------------------------
CGLMeshBuffer::CGLMeshBuffer()
    : m_eUsagePattern(eStaticDraw)
    , m_ePrimitiveType(ePrimitiveTriangles)
    , m_uiPrimitiveIndex(0)
    , m_iIndicesCount(0)
    , m_bUseVertexArray(false)
{
    initializeOpenGLFunctions();
}

//-------------------------------------------------------------------------------------------------
CGLMeshBuffer::~CGLMeshBuffer()
{
    m_PositionBuffer.destroy();
    m_NormalBuffer.destroy();
    m_TexCoordBuffer.destroy();
    m_TangentBuffer.destroy();
    m_BoneIDsBuffer.destroy();
    m_BoneWeigthsBuffer.destroy();
    m_IndiceBuffer.destroy();
}


//-------------------------------------------------------------------------------------------------
void CGLMeshBuffer::create(CMeshBuffer* pBuffer)
{
    m_iIndicesCount = pBuffer->indicesBuffer().size();
    m_ePrimitiveType = pBuffer->getPrimitiveType();
    m_eUsagePattern = pBuffer->getUsagePattern();

    if (m_bUseVertexArray)
    {
        m_VertexArray.create();
        m_VertexArray.bind();
    }

    // Positions
    copyBuffer<QVector3D>(pBuffer->positionsBuffer(), m_PositionBuffer, CGLBuffer::VertexBuffer);

    // Normals
    copyBuffer<QVector3D>(pBuffer->normalsBuffer(), m_NormalBuffer, CGLBuffer::VertexBuffer);

    // TexCoord
    copyBuffer<QVector3D>(pBuffer->texCoordBuffer(0), m_TexCoordBuffer, CGLBuffer::VertexBuffer);

    // Tangent
    copyBuffer<QVector3D>(pBuffer->tangentsBuffer(), m_TangentBuffer, CGLBuffer::VertexBuffer);

    // Bones IDs
    copyBuffer<float>(pBuffer->boneIDsBuffer(), m_BoneIDsBuffer, CGLBuffer::VertexBuffer);

    // Bones Weights
    copyBuffer<float>(pBuffer->boneWeightsBuffer(), m_BoneWeigthsBuffer, CGLBuffer::VertexBuffer);

    // User Buffers
    QMapIterator<QString, CUserBuffer> it(pBuffer->userBuffers());
    while (it.hasNext())
    {
        it.next();
        CGLBuffer buffer;
        m_UserBuffers[it.key()] = buffer;

        copyBuffer<float>(it.value(), buffer, CGLBuffer::VertexBuffer);
    }

    // Deselect (bind to 0) the VAO
    if (m_bUseVertexArray)
    {
        m_VertexArray.release();
    }

    // Create a new VBO for the indices and select it (bind) - INDICES
    copyBuffer<IndiceType>(pBuffer->indicesBuffer(), m_IndiceBuffer, CGLBuffer::IndexBuffer);
}

//-------------------------------------------------------------------------------------------------
void CGLMeshBuffer::update(CMeshBuffer* pBuffer)
{
    // Positions
    updateBuffer<QVector3D>(pBuffer->positionsBuffer(), m_PositionBuffer);

    // Normals
    updateBuffer<QVector3D>(pBuffer->normalsBuffer(), m_NormalBuffer);

    // TexCoords
    updateBuffer<QVector3D>(pBuffer->texCoordBuffer(0), m_TexCoordBuffer);

    // Tangents
    updateBuffer<QVector3D>(pBuffer->tangentsBuffer(), m_TexCoordBuffer);

    // Bones IDs
    updateBuffer<float>(pBuffer->boneIDsBuffer(), m_BoneIDsBuffer);

    // Bones Weights
    updateBuffer<float>(pBuffer->boneWeightsBuffer(), m_BoneWeigthsBuffer);

    // Indices
    updateBuffer<IndiceType>(pBuffer->indicesBuffer(), m_IndiceBuffer);

    // User buffers
    QMapIterator<QString, CUserBuffer> it(pBuffer->userBuffers());
    while (it.hasNext())
    {
        it.next();
        CGLBuffer userGLBuffer = m_UserBuffers[it.key()];

        updateBuffer<float>(it.value(), userGLBuffer);
    }
}

//-------------------------------------------------------------------------------------------------
void CGLMeshBuffer::setPrimitiveType(EnumPrimitiveType ePrimitiveType)
{
    m_ePrimitiveType = ePrimitiveType;
}

//-------------------------------------------------------------------------------------------------
void CGLMeshBuffer::setPrimitiveIndex(unsigned int uiPrimitiveIndex)
{
    m_uiPrimitiveIndex = uiPrimitiveIndex;
}

//-------------------------------------------------------------------------------------------------
void CGLMeshBuffer::draw(CGLShaderProgram* pShaderProgram)
{
    if (m_IndiceBuffer.isCreated())
    {
        if (m_bUseVertexArray)
        {
            m_VertexArray.bind();
        }

        bind(pShaderProgram, m_PositionBuffer, PROGRAM_POSITION_ATTRIBUTE, 3, GL_FLOAT);
        bind(pShaderProgram, m_NormalBuffer, PROGRAM_NORMAL_ATTRIBUTE, 3, GL_FLOAT);
        bind(pShaderProgram, m_TexCoordBuffer, PROGRAM_TEXTURE_ATTRIBUTE, 3, GL_FLOAT);
        bind(pShaderProgram, m_TangentBuffer, PROGRAM_TANGENT_ATTRIBUTE, 3, GL_FLOAT);
        bind(pShaderProgram, m_BoneIDsBuffer, PROGRAM_BONE_IDS_ATTRIBUTE, DefaultMaxBonesPerVertex, GL_FLOAT);
        bind(pShaderProgram, m_BoneWeigthsBuffer, PROGRAM_BONE_WEIGHTS_ATTRIBUTE, DefaultMaxBonesPerVertex, GL_FLOAT);


        // User VBO
        QMapIterator<QString, CGLBuffer> itBind(m_UserBuffers);
        int iCurrentUserBuffer = 0;
        while (itBind.hasNext())
        {
            itBind.next();
            CGLBuffer userGLBuffer = itBind.value();
            bind(pShaderProgram, userGLBuffer, PROGRAM_USER_ATTRIBUTES + iCurrentUserBuffer, 3, GL_FLOAT);
            ++iCurrentUserBuffer;
        }

        m_IndiceBuffer.bind();
        // TODO : if (m_uiPrimitiveIndex > 0) glPrimitiveRestartIndex(m_uiPrimitiveIndex);
        glDrawElements(CGLHelper::toGLType(m_ePrimitiveType), m_iIndicesCount, s_IndiceValuesType, 0);
        m_IndiceBuffer.release();

        release(pShaderProgram, m_PositionBuffer, PROGRAM_POSITION_ATTRIBUTE);
        release(pShaderProgram, m_NormalBuffer, PROGRAM_NORMAL_ATTRIBUTE);
        release(pShaderProgram, m_TexCoordBuffer, PROGRAM_TEXTURE_ATTRIBUTE);
        release(pShaderProgram, m_TexCoordBuffer, PROGRAM_TANGENT_ATTRIBUTE);
        release(pShaderProgram, m_BoneIDsBuffer, PROGRAM_BONE_IDS_ATTRIBUTE);
        release(pShaderProgram, m_BoneWeigthsBuffer, PROGRAM_BONE_WEIGHTS_ATTRIBUTE);

        // User VBO
        QMapIterator<QString, CGLBuffer> itRelease(m_UserBuffers);
        iCurrentUserBuffer = 0;
        while (itRelease.hasNext())
        {
            itRelease.next();
            CGLBuffer userGLBuffer = itRelease.value();
            release(pShaderProgram, userGLBuffer, PROGRAM_USER_ATTRIBUTES + iCurrentUserBuffer);
            ++iCurrentUserBuffer;
        }

        if (m_bUseVertexArray)
        {
            m_VertexArray.release();
        }
    }
}

