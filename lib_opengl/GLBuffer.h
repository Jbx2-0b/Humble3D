#ifndef GLBUFFER_H
#define GLBUFFER_H

class GLBuffer
{
public:
	GLBuffer(EnumBufferType eType) {
		m_eType = eType;
		m_eUsagePattern = EnumUsagePattern.eStaticDraw;
		m_iVBOID = 0;
	}

	~GLBUffer()
	{
		destroy();
	}
	
	EnumBufferType getType() const {
		return m_eType;
	}

	EnumUsagePattern getUsagePattern() const {
		return m_eUsagePattern;
	}

	void setUsagePattern(EnumUsagePattern eUsagePattern){
		m_eUsagePattern = eUsagePattern;
	}

	void create() {
		m_iVBOID = glGenBuffers();
	}

	bool isCreated() {
		return m_iVBOID > 0;
	}

	void destroy() {
		if (isCreated()) {
			// Delete the vertex VBO
			glBindBuffer(GLTypeConverter.toGLType(m_eType), 0);
			glDeleteBuffers(m_iVBOID);
			m_iVBOID = 0;
		}
	}

	void bind() {
		if (isCreated())
		{
			glBindBuffer(GLTypeConverter.toGLType(m_eType), m_iVBOID);
		}
	}

	void release() {
		if (isCreated())
			glBindBuffer(GLTypeConverter.toGLType(m_eType), 0);
	}

	static void release(EnumBufferType eType) {
		glBindBuffer(GLTypeConverter.toGLType(eType), 0);
	}

	int bufferId() {
		return m_iVBOID;
	}

	int size() {
		return glGetBufferParameteri(GLTypeConverter.toGLType(m_eType),
				GL_BUFFER_SIZE);
	}

	<T> void read(int offset, T data) {
		if (data instanceof ByteBuffer)
			glGetBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(ByteBuffer) data);
		else if (data instanceof IntBuffer)
			glGetBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(IntBuffer) data);
		else if (data instanceof FloatBuffer)
			glGetBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(FloatBuffer) data);
		else if (data instanceof DoubleBuffer)
			glGetBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(DoubleBuffer) data);
		else if (data instanceof ShortBuffer)
			glGetBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(ShortBuffer) data);
	}

	<T> void write(int offset, T data) {	
		if (data instanceof ByteBuffer)
			glBufferSubData(GLTypeConverter.toGLType(m_eType), offset, (ByteBuffer) data);
		else if (data instanceof ShortBuffer)
			glBufferSubData(GLTypeConverter.toGLType(m_eType), offset, (ShortBuffer) data);
		else if (data instanceof IntBuffer)
			glBufferSubData(GLTypeConverter.toGLType(m_eType), offset, (IntBuffer) data);
		else if (data instanceof FloatBuffer)
			glBufferSubData(GLTypeConverter.toGLType(m_eType), offset, (FloatBuffer) data);
		else if (data instanceof DoubleBuffer)
			glBufferSubData(GLTypeConverter.toGLType(m_eType), offset,
					(DoubleBuffer) data);
	}

	<T> void allocate(T data) {
		if (data instanceof ByteBuffer)
			glBufferData(GLTypeConverter.toGLType(m_eType), (ByteBuffer) data,
					GLTypeConverter.toGLType(m_eUsagePattern));
		else if (data instanceof ShortBuffer)
			glBufferData(GLTypeConverter.toGLType(m_eType), (ShortBuffer) data,
					GLTypeConverter.toGLType(m_eUsagePattern));
		else if (data instanceof IntBuffer)
			glBufferData(GLTypeConverter.toGLType(m_eType), (IntBuffer) data,
					GLTypeConverter.toGLType(m_eUsagePattern));
		else if (data instanceof FloatBuffer)
			glBufferData(GLTypeConverter.toGLType(m_eType), (FloatBuffer) data,
					GLTypeConverter.toGLType(m_eUsagePattern));
		else if (data instanceof DoubleBuffer)
			glBufferData(GLTypeConverter.toGLType(m_eType), (DoubleBuffer) data,
					GLTypeConverter.toGLType(m_eUsagePattern));
	}

protected:
	EnumBufferType m_eType;
	EnumUsagePattern m_eUsagePattern;
	int m_iVBOID;

}


#endif // GLBUFFER_H