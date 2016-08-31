#include "GLBuffer.h"

//-------------------------------------------------------------------------------------------------
GLBuffer::GLBuffer(EnumBufferType eType)
{
	m_eType = eType;
	m_eUsagePattern = EnumUsagePattern.eStaticDraw;
	m_iVBOID = 0;
}

//-------------------------------------------------------------------------------------------------
GLBuffer::~GLBuffer()
{
	destroy();
}

//-------------------------------------------------------------------------------------------------
EnumBufferType GLBuffer::getType() const {
	return m_eType;
}

//-------------------------------------------------------------------------------------------------
EnumUsagePattern GLBuffer::getUsagePattern() const {
	return m_eUsagePattern;
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::setUsagePattern(EnumUsagePattern eUsagePattern){
	m_eUsagePattern = eUsagePattern;
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::create() {
	m_iVBOID = glGenBuffers();
}

//-------------------------------------------------------------------------------------------------
bool GLBuffer::isCreated() {
	return m_iVBOID > 0;
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::destroy() {
	if (isCreated()) {
		// Delete the vertex VBO
		glBindBuffer(GLTypeConverter.toGLType(m_eType), 0);
		glDeleteBuffers(m_iVBOID);
		m_iVBOID = 0;
	}
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::bind() {
	if (isCreated())
	{
		glBindBuffer(GLTypeConverter.toGLType(m_eType), m_iVBOID);
	}
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::release() {
	if (isCreated())
		glBindBuffer(GLTypeConverter.toGLType(m_eType), 0);
}

//-------------------------------------------------------------------------------------------------
void GLBuffer::release(EnumBufferType eType) {
	glBindBuffer(GLTypeConverter.toGLType(eType), 0);
}

//-------------------------------------------------------------------------------------------------
int GLBuffer::bufferId() {
	return m_iVBOID;
}

//-------------------------------------------------------------------------------------------------
int GLBuffer::size() {
	return glGetBufferParameteri(GLTypeConverter.toGLType(m_eType),
		GL_BUFFER_SIZE);
}
