package opengl.renderer;

import geometry.buffers.AIndexBuffer;
import geometry.buffers.AVertexBuffer;
import geometry.buffers.UniformBuffer;
import geometry.renderer.ITexture2D;
import geometry.renderstates.EnumUsagePattern;
import geometry.shaders.IShaderProgram;

import java.awt.image.BufferedImage;

import opengl.buffers.GLIndexBuffer;
import opengl.buffers.GLVertexBuffer;
import opengl.shaders.GLShaderProgram;
import opengl.textures.GLTexture2D;
import todo.IDevice;

class GLDevice implements IDevice {

	//! Override
	IShaderProgram createShaderProgram(String vertexShaderSource,
			String geometryShaderSource, String fragmentShaderSource) {

		return new GLShaderProgram(vertexShaderSource, geometryShaderSource,
				fragmentShaderSource);
	}

	//! Override
	IShaderProgram createShaderProgram(String vertexShaderSource,
			String fragmentShaderSource) {

		return new GLShaderProgram(vertexShaderSource, fragmentShaderSource);
	}

	//! Override
	AVertexBuffer createVertexBuffer(EnumUsagePattern usagePattern) {

		return new GLVertexBuffer(usagePattern);
	}

	//! Override
	AIndexBuffer createIndexBuffer(EnumUsagePattern usagePattern) {

		return new GLIndexBuffer(usagePattern);
	}

	//! Override
	ITexture2D createTexture2D(String fileName) {
		return new GLTexture2D(fileName);
	}

	//! Override
	ITexture2D createTexture2D(BufferedImage bitmap) {
		return new GLTexture2D(bitmap);
	}

	//! Override
	UniformBuffer createUniformBuffer(EnumUsagePattern usagePattern) {
		// TODO Auto-generated method stub
		return null;
	}

}
