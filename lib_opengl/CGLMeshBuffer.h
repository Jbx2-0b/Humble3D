#ifndef CGLMESHBUFFER_H
#define CGLMESHBUFFER_H

#include "lib_opengl_global.h"

#include "CGLHelper.h"
#include "CMeshBuffer.h"
#include "ALockable.h"

class CGLRenderer;

// Qt
#include <QMap>

#ifdef EMBEDDED_TARGET
static const char* strAttributes[] =
{
    "vertexPosition",
    "vertexNormal",
    "vertexTexCoord",
    "vertexTangent",
    "vertexBoneIDs",
    "vertexPosition",
    "vertexWeights",
    "userAttributes"
};
#endif // EMBEDDED_TARGET


class CGLMeshBuffer : public ALockable, protected CGLFunctions
{
	enum EnumAttributes
	{ 
		PROGRAM_POSITION_ATTRIBUTE, 
		PROGRAM_NORMAL_ATTRIBUTE, 
		PROGRAM_TEXTURE_ATTRIBUTE, 
		PROGRAM_TANGENT_ATTRIBUTE, 
		PROGRAM_BONE_IDS_ATTRIBUTE,
		PROGRAM_BONE_WEIGHTS_ATTRIBUTE,
		PROGRAM_USER_ATTRIBUTES
	};



public:

	//! Constructeur
	CGLMeshBuffer();

	//! Destructeur
	~CGLMeshBuffer();

    //! COnstruit en fonction d'un MeshBuffer
    void create(CMeshBuffer* pBuffer);

	//! Met à jour le buffer
	void update(CMeshBuffer* pBuffer);

	//! Définit le type de primitive
	void setPrimitiveType(EnumPrimitiveType ePrimitiveType);

    //! Définit le nombre de côté par face
    void setPrimitiveIndex(unsigned int uiPrimitiveIndex);

	//! Effectue le rendu du buffer
	void draw(CGLShaderProgram* pShaderProgram);

    //! Active ou desactive les vertex arrays
    void setVertexArrayEnabled(bool bEnabled) { m_bUseVertexArray = bEnabled; }

private:

	template<class T> void copyBuffer(const CBuffer<T>& inBuffer, CGLBuffer& outBuffer, CGLBuffer::Type eTypeBuffer)
	{
		if (inBuffer.size() > 0)
		{
			outBuffer = CGLBuffer(eTypeBuffer);
            outBuffer.setUsagePattern(CGLHelper::toQtType(m_eUsagePattern));
			outBuffer.create();
            outBuffer.bind();
			outBuffer.allocate(inBuffer.constData(), inBuffer.getSize());
			outBuffer.release();
		}
	}

	template<class T> void updateBuffer(const CBuffer<T>& inBuffer, CGLBuffer& outBuffer)
	{
        if ((inBuffer.size() > 0) && inBuffer.isDirty() && outBuffer.isCreated())
		{
            outBuffer.bind();
			outBuffer.write(0, inBuffer.constData(), inBuffer.getSize());
			outBuffer.release();
		}
	}

	void bind(CGLShaderProgram* pShaderProgram, CGLBuffer& buffer, int iLocation, int iTupleSize, int iType)
	{
		if (buffer.isCreated())
        {
            buffer.bind();

#ifdef DESKTOP_TARGET
            pShaderProgram->enableAttributeArray(iLocation);
            pShaderProgram->setAttributeBuffer(iLocation, iType, 0, iTupleSize);
#else // EMBEDDED_TARGET
            pShaderProgram->enableAttributeArray(strAttributes[iLocation]);
            pShaderProgram->setAttributeBuffer(strAttributes[iLocation], iType, 0, iTupleSize);
#endif
			buffer.release();
		}
	}

    void bind(CGLShaderProgram* pShaderProgram, CGLBuffer& buffer, const QString& name, int iTupleSize, int iType)
    {
        if (buffer.isCreated())
        {
            buffer.bind();
            pShaderProgram->enableAttributeArray(name.toLatin1().constData());
            pShaderProgram->setAttributeBuffer(name.toLatin1().constData(), iType, 0, iTupleSize);
            buffer.release();
        }
    }

	void release(CGLShaderProgram* pShaderProgram, CGLBuffer& buffer, int iLocation)
	{
		if (buffer.isCreated()) 
			pShaderProgram->disableAttributeArray(iLocation);
	}

	EnumUsagePattern m_eUsagePattern;
	EnumPrimitiveType m_ePrimitiveType;
    unsigned int m_uiPrimitiveIndex;
	int m_iIndicesCount;

	CGLVertexArrayObject m_VertexArray;
    bool m_bUseVertexArray;
	CGLBuffer m_PositionBuffer;
    CGLBuffer m_NormalBuffer;
    CGLBuffer m_TexCoordBuffer;
    CGLBuffer m_TangentBuffer;
	CGLBuffer m_BoneIDsBuffer;
	CGLBuffer m_BoneWeigthsBuffer;
	CGLBuffer m_IndiceBuffer;
	QMap<QString, CGLBuffer> m_UserBuffers;
};


#endif // CGLMESHBUFFER_H
