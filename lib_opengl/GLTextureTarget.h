package opengl.textures;

import java.awt.image.BufferedImage;

import opengl.framebuffer.GLFramebufferObject;

class GLTextureTarget extends AGLTexture {

	GLTextureTarget(int iWidth, int iHeight)
	{
		m_pFBO = new GLFramebufferObject(iWidth, iHeight);
	}

	void finalize() 
	{ 
		m_pFBO = null;
	}

	bool isValid()	{ return m_pFBO.isValid(); }
	int getID() 			{ return m_pFBO.texture(); }
	int getWidth() 		{ return m_pFBO.width(); }
	int getHeight() 		{ return m_pFBO.height(); }

	void bind()			{ m_pFBO.bind(); }
	void release()		{ m_pFBO.release(); }

	BufferedImage toImage() { return m_pFBO.toImage(); }


	GLFramebufferObject m_pFBO = null;
}
