package opengl.buffers;

import geometry.buffers.AMemoryBuffer;
import geometry.buffers.MeshBuffer;
import geometry.renderstates.EnumPrimitiveType;
import geometry.renderstates.EnumUsagePattern;
import geometry.shaders.IShaderProgram;

import opengl.EnumBufferType;
import opengl.GLTypeConverter;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;

class GLMeshBuffer {

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
	
	
	GLMeshBuffer(MeshBuffer pBuffer)
	{
		indicesCount = pBuffer.indicesBuffer().size();
		m_ePrimitiveType = pBuffer.getPrimitiveType();
		m_eUsagePattern = pBuffer.getUsagePattern();
						
		vertexArray = new GLVertexArrayObject();
		vertexArray.create();
		vertexArray.bind();

		// Positions
		positionBuffer = createBuffer(pBuffer.positionsBuffer(), EnumBufferType.eVertexBuffer);

		// TexCoord
		texCoordBuffer = createBuffer(pBuffer.texCoordBuffer(0), EnumBufferType.eVertexBuffer);

		// Normals
		normalBuffer = createBuffer(pBuffer.normalsBuffer(), EnumBufferType.eVertexBuffer);

		// Bones IDs
		boneIDsBuffer = createBuffer(pBuffer.boneIDsBuffer(), EnumBufferType.eVertexBuffer);

		// Bones Weights
		boneWeigthsBuffer = createBuffer(pBuffer.boneWeightsBuffer(), EnumBufferType.eVertexBuffer);

		// Deselect (bind to 0) the VAO
		vertexArray.release();
		
		// Indices
		indiceBuffer = createBuffer(pBuffer.indicesBuffer(), EnumBufferType.eIndexBuffer);
	}
	
	void setPrimitiveType(EnumPrimitiveType ePrimitiveType)
	{
		m_ePrimitiveType = ePrimitiveType;
	}
	
	void draw(IShaderProgram shaderProgram)
	{
		// Bind to the VAO that has all the information about the vertices
		vertexArray.bind();
		
		bind(shaderProgram, positionBuffer, EnumAttributes.PROGRAM_POSITION_ATTRIBUTE.ordinal(), MeshBuffer.positionElementCount, GL_FLOAT);
		bind(shaderProgram, texCoordBuffer, EnumAttributes.PROGRAM_TEXTURE_ATTRIBUTE.ordinal(), MeshBuffer.textureElementCount, GL_FLOAT);
		bind(shaderProgram, normalBuffer, EnumAttributes.PROGRAM_NORMAL_ATTRIBUTE.ordinal(), MeshBuffer.normalElementCount, GL_FLOAT);
		bind(shaderProgram, boneIDsBuffer, EnumAttributes.PROGRAM_BONE_IDS_ATTRIBUTE.ordinal(), MeshBuffer.boneIDElementCount, GL_UNSIGNED_INT);
		bind(shaderProgram, boneWeigthsBuffer, EnumAttributes.PROGRAM_BONE_WEIGHTS_ATTRIBUTE.ordinal(), MeshBuffer.boneWeigthElementCount, GL_FLOAT);
		
		indiceBuffer.bind();
		// Draw the vertices
		glDrawElements(GLTypeConverter.toGLType(m_ePrimitiveType), indicesCount, GL_UNSIGNED_INT, 0);
		indiceBuffer.release();
		
		release(positionBuffer, EnumAttributes.PROGRAM_POSITION_ATTRIBUTE.ordinal());
		release(texCoordBuffer, EnumAttributes.PROGRAM_TEXTURE_ATTRIBUTE.ordinal());
		release(normalBuffer, EnumAttributes.PROGRAM_NORMAL_ATTRIBUTE.ordinal());
		release(boneIDsBuffer, EnumAttributes.PROGRAM_BONE_IDS_ATTRIBUTE.ordinal());
		release(boneWeigthsBuffer, EnumAttributes.PROGRAM_BONE_WEIGHTS_ATTRIBUTE.ordinal());
		
		vertexArray.release();
	}
	
	void update(MeshBuffer pBuffer)
	{
		// Positions
		updateBuffer(pBuffer.positionsBuffer(), positionBuffer);

		// TexCoord
		updateBuffer(pBuffer.texCoordBuffer(0), texCoordBuffer);

		// Normals
		updateBuffer(pBuffer.normalsBuffer(), normalBuffer);

		// Bones IDs
		updateBuffer(pBuffer.boneIDsBuffer(), boneIDsBuffer);

		// Bones Weights
		updateBuffer(pBuffer.boneWeightsBuffer(), boneWeigthsBuffer);

		// Indices
		updateBuffer(pBuffer.indicesBuffer(), indiceBuffer);	
	}
	
	void delete()
	{
		// Select the VAO
		vertexArray.bind();
		
		// Disable the VBO index from the VAO attributes list
		GL20.glDisableVertexAttribArray(0);
		if (texCoordBuffer != null) GL20.glDisableVertexAttribArray(1);
		if (normalBuffer != null) GL20.glDisableVertexAttribArray(2);
		
		positionBuffer.destroy();
		
		if (texCoordBuffer != null) texCoordBuffer.destroy();
		if (normalBuffer != null) normalBuffer.destroy();
		if (boneIDsBuffer != null) boneIDsBuffer.destroy();
		if (boneWeigthsBuffer != null) boneWeigthsBuffer.destroy();
		
		// Delete the index VBO
		indiceBuffer.destroy();
		
		// Delete the VAO
		vertexArray.destroy();
	}
	
	void finalize()
	{
		delete();
	}
	
	GLBuffer createBuffer(AMemoryBuffer<?> inBuffer, EnumBufferType eTypeBuffer)
	{
		GLBuffer resultBuffer = null;
		if (inBuffer.size() > 0)
		{
			resultBuffer = new GLBuffer(eTypeBuffer);
			resultBuffer.setUsagePattern(m_eUsagePattern);
			resultBuffer.create();
			resultBuffer.bind();
			resultBuffer.allocate(inBuffer.toBuffer());
			resultBuffer.release();
		}
		
		return resultBuffer;
	}

	void updateBuffer(AMemoryBuffer<?> inBuffer, GLBuffer outBuffer)
	{
		if ((inBuffer.size() > 0) && inBuffer.isDirty())
		{
			outBuffer.bind();
			outBuffer.write(0, inBuffer.toBuffer());
			outBuffer.release();
		}
	}
	
	void bind(IShaderProgram pShaderProgram, GLBuffer buffer, int iLocation, int iTupleSize, int iType)
	{
		if (buffer != null)
		{
			buffer.bind();
			GL20.glEnableVertexAttribArray(iLocation);
			pShaderProgram.setAttributeBuffer(iLocation, iType, 0, iTupleSize, 0);
			buffer.release();
		}
	}

	void release(GLBuffer buffer, int iLocation)
	{
		if (buffer != null)
		{
			GL20.glDisableVertexAttribArray(iLocation);
		}
	}

	EnumUsagePattern m_eUsagePattern;
	int indicesCount;
	EnumPrimitiveType m_ePrimitiveType = EnumPrimitiveType.ePrimitivePoint;
	GLVertexArrayObject vertexArray;
	GLBuffer positionBuffer = null;
	GLBuffer texCoordBuffer = null;
	GLBuffer normalBuffer = null;
	GLBuffer boneIDsBuffer = null;
	GLBuffer boneWeigthsBuffer = null;
	GLBuffer indiceBuffer = null;
	
	
}
