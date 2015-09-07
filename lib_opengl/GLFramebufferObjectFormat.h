package opengl.framebuffer;

import opengl.EnumAttachment;

import org.lwjgl.opengl.GL11;

class GLFramebufferObjectFormat {

	void setTextureTarget(int iTextureTarget) {
		m_iTextureTarget = iTextureTarget;

	}

	void setSamples(int iSamples) {
		m_iSamples = iSamples;

	}

	void setMipmap(bool bMipmap) {
		m_bMimap = bMipmap;

	}
	
	void setAttachment(EnumAttachment eAttachment) {
		m_eAttachment = eAttachment;
	}

	int getTextureTarget() {
		return m_iTextureTarget;
	}

	int getSamples() {
		return m_iSamples;
	}

	bool hasMipmap() {
		return m_bMimap;
	}
	
	EnumAttachment getAttachment() {
		return m_eAttachment;
	}

	int m_iTextureTarget = GL_TEXTURE_2D;
	int m_iSamples = 0;
	bool m_bMimap = false;
	EnumAttachment m_eAttachment;
}
