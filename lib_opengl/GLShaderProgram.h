package opengl.shaders;

import geometry.shaders.IShaderProgram;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;

import math.matrices.QMatrix3x3;
import math.matrices.QMatrix4x4;
import math.vector.QVector3D;
import math.vector.QVector4D;

import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL20;
import org.lwjgl.opengl.GL32;

class GLShaderProgram implements IShaderProgram {
	static final int INVALID_SHADER_ID = -1;
	private int vsID = INVALID_SHADER_ID;
	private int fsID = INVALID_SHADER_ID;
	private int gsID = INVALID_SHADER_ID;
	private int pID = 0;
	private HashMap<String, Integer> uniformLocation = new HashMap<String, Integer>();
	private FloatBuffer matrix33Buffer = BufferUtils.createFloatBuffer(9);
	private FloatBuffer matrix44Buffer = BufferUtils.createFloatBuffer(16);
	bool linked = false;
	bool binded = false;

	GLShaderProgram(String vertexFileName, String geometryFileName, String fragmentFileName)
	{
		// Load the vertex shader
		if (!vertexFileName.isEmpty()) 
			vsID = this.loadShader(vertexFileName, GL20.GL_VERTEX_SHADER);
		// Load the fragment shader
		if (!geometryFileName.isEmpty()) 
			gsID = this.loadShader(geometryFileName, GL32.GL_GEOMETRY_SHADER);
		// Load the fragment shader
		if (!fragmentFileName.isEmpty()) 
			fsID = this.loadShader(fragmentFileName, GL20.GL_FRAGMENT_SHADER);

		pID = GL20.glCreateProgram();
		if (vsID != INVALID_SHADER_ID)
		{
			GL20.glAttachShader(pID, vsID);
		}
		
		if (gsID != INVALID_SHADER_ID)
		{
			GL20.glAttachShader(pID, gsID);
		}
		
		if (fsID != INVALID_SHADER_ID)
		{
			GL20.glAttachShader(pID, fsID);
		}
	}

	GLShaderProgram(String vertexFileName, String fragmentFileName) {
		// Load the vertex shader
		vsID = loadShader(vertexFileName, GL20.GL_VERTEX_SHADER);
		// Load the fragment shader
		fsID = loadShader(fragmentFileName, GL20.GL_FRAGMENT_SHADER);

		pID = GL20.glCreateProgram();
		if (vsID != INVALID_SHADER_ID)
		{
			GL20.glAttachShader(pID, vsID);
		}
		
		if (fsID != INVALID_SHADER_ID)
		{
			GL20.glAttachShader(pID, fsID);
		}
	}

	void bind() {
		if (!linked) {
			link();
		}

		binded = true;
		GL20.glUseProgram(pID);
	}

	void link() {
		linked = true;
		GL20.glLinkProgram(pID);
		GL20.glValidateProgram(pID);
	}

	void release() {
		if (binded) {
			GL20.glUseProgram(0);
			binded = false;
		}
	}

	void delete() {
		GL20.glUseProgram(0);
		GL20.glDeleteProgram(pID);
	}

	void bindAttributeLocation(int index, String name) {
		GL20.glBindAttribLocation(pID, index, name);
	}

	int addUniformValue(String name) {
		int location = GL20.glGetUniformLocation(pID, name);
		uniformLocation.put(name, location);
		return location;
	}

	void setUniformValue(String name, bool value) {
		int location = getUniformLocation(name);
		GL20.glUniform1i(location, value ? 1 : 0);
	}
	
	void setUniformValue(String name, int value) {
		int location = getUniformLocation(name);
		GL20.glUniform1i(location, value);
	}
	
	void setUniformValue(String name, float value) {
		int location = getUniformLocation(name);
		GL20.glUniform1f(location, value);
	}
	
	void setUniformValue(String name, double value) {
		int location = getUniformLocation(name);
		GL20.glUniform1f(location, (float)value);
	}
	
	void setUniformValue(String name, QVector3D value) {
		int location = getUniformLocation(name);
		GL20.glUniform3f(location, (float) value.x(), (float) value.y(),
				(float) value.z());
	}

	void setUniformValue(int location, QVector3D value) {
		GL20.glUniform3f(location, (float) value.x(), (float) value.y(),
				(float) value.z());
	}

	void setUniformValue(String name, QVector4D value) {
		int location = getUniformLocation(name);
		GL20.glUniform4f(location, (float) value.x(), (float) value.y(),
				(float) value.z(), (float) value.w());
	}

	void setUniformValue(int location, QVector4D value) {
		GL20.glUniform4f(location, (float) value.x(), (float) value.y(),
				(float) value.z(), (float) value.w());
	}

	void setUniformValue(String name, QMatrix3x3 value) {
		int location = getUniformLocation(name);
		value.toFLoatBuffer(matrix33Buffer);
		matrix33Buffer.flip();
		GL20.glUniformMatrix3(location, false, matrix33Buffer);
	}

	void setUniformValue(int location, QMatrix3x3 value) {
		value.toFLoatBuffer(matrix33Buffer);
		matrix33Buffer.flip();
		GL20.glUniformMatrix3(location, false, matrix33Buffer);
	}

	void setUniformValue(String name, QMatrix4x4 value) {
		int location = getUniformLocation(name);
		value.toFLoatBuffer(matrix44Buffer);
		matrix44Buffer.flip();
		GL20.glUniformMatrix4(location, false, matrix44Buffer);

	}

	void setUniformValue(int location, QMatrix4x4 value) {
		value.toFLoatBuffer(matrix44Buffer);
		matrix44Buffer.flip();
		GL20.glUniformMatrix4(location, false, matrix44Buffer);
	}
	
	void setUniformValueArray(int location, IntBuffer buffer) {
		buffer.flip();
		GL20.glUniform1(location, buffer);
	}
	
	void setUniformValueArray(String name, IntBuffer buffer) {
		setUniformValueArray(getUniformLocation(name), buffer);
	}
	
	void setUniformValueArray(int location, FloatBuffer buffer) {
		buffer.flip();
		GL20.glUniform1(location, buffer);
	}
	
	void setUniformValueArray(String name, FloatBuffer buffer) {
		setUniformValueArray(getUniformLocation(name), buffer);
	}
	
	private int getUniformLocation(String name)
	{
		if(uniformLocation.containsKey(name))
		{
			 return uniformLocation.get(name);
		}
		return addUniformValue(name);
	}

	private int loadShader(String filename, int type) {
		StringBuilder shaderSource = new StringBuilder();
		int shaderID = 0;

		try {
			BufferedReader reader = new BufferedReader(new FileReader(filename));
			String line;
			while ((line = reader.readLine()) != null) {
				shaderSource.append(line).append("\n");
			}
			reader.close();
		} catch (IOException e) {
			System.err.println("Could not read file.");
			e.printStackTrace();
			return INVALID_SHADER_ID;
		}

		shaderID = GL20.glCreateShader(type);
		GL20.glShaderSource(shaderID, shaderSource);
		GL20.glCompileShader(shaderID);

		if (GL20.glGetShaderi(shaderID, GL20.GL_COMPILE_STATUS) == GL_FALSE)
		{
			System.err.println("Could not compile shader.");
			int length = GL20.glGetShaderi(shaderID, GL20.GL_INFO_LOG_LENGTH);
			System.out.println(GL20.glGetShaderInfoLog(vsID, length));
			
			return INVALID_SHADER_ID;
		}

		return shaderID;
	}

	//! Override
	void setAttributeBuffer(int location, int type, int offset,
			int tupleSize, int stride) {
		GL20.glVertexAttribPointer(location, tupleSize, type, false, stride,
				offset);

	}
}
