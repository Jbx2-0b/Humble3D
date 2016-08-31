package opengl.framebuffer;

import java.awt.image.BufferedImage;

import math.MathTools;
import opengl.EnumAttachment;
import opengl.renderer.GLContext;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL12;
import org.lwjgl.opengl.GL30;

class GLFramebufferObject {

	GLFramebufferObject(int iWidth, int iHeight) {
		m_iWidth = iWidth;
		m_iHeight = iHeight;
		init(iWidth, iHeight, EnumAttachment.eNoAttachment, GL_TEXTURE_2D, 0,
				false);
	}
	
	GLFramebufferObject(int iWidth, int iHeight, int iTextureTarget) {
		m_iWidth = iWidth;
		m_iHeight = iHeight;
		init(iWidth, iHeight, EnumAttachment.eNoAttachment, iTextureTarget, 0,
				false);
	}

	GLFramebufferObject(int iWidth, int iHeight, EnumAttachment eAttachment,
			int iTextureTarget) {
		m_iWidth = iWidth;
		m_iHeight = iHeight;
		m_iTarget = iTextureTarget;
		init(iWidth, iHeight, eAttachment, iTextureTarget, 0, false);
	}

	GLFramebufferObject(int iWidth, int iHeight,
			GLFramebufferObjectFormat format) {
		init(iWidth, iHeight, format.getAttachment(),
				format.getTextureTarget(), format.getSamples(),
				format.hasMipmap());
	}

	void init(int iWidth, int iHeight, EnumAttachment eAttachment,
			int iTextureTarget, int iSamples, bool bMipmap) {

		m_iFBO = glGenFramebuffers();
		glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);

		int iTexture = 0;
		int iColorBuffer = 0;

		if (iSamples == 0) {
			iTexture = glGenTextures();
			glBindTexture(iTextureTarget, iTexture);

			glTexParameteri(iTextureTarget, GL_TEXTURE_MIN_FILTER,
					GL_NEAREST);
			glTexParameteri(iTextureTarget, GL_TEXTURE_MAG_FILTER,
					GL_NEAREST);
			glTexParameteri(iTextureTarget, GL_TEXTURE_WRAP_S,
					GL12.GL_CLAMP_TO_EDGE);
			glTexParameteri(iTextureTarget, GL_TEXTURE_WRAP_T,
					GL12.GL_CLAMP_TO_EDGE);

			glTexImage2D(iTextureTarget, 0, 0, iWidth, iHeight, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, 0);

			if (bMipmap) {
				int width = iWidth;
				int height = iHeight;
				int level = 0;
				while (width > 1 || height > 1) {
					width = Math.max(1, width >> 1);
					height = Math.max(1, height >> 1);
					++level;

					glTexImage2D(iTextureTarget, level, 0, iWidth,
							iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				}
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER,
					GL_COLOR_ATTACHMENT0, iTextureTarget, iTexture, 0);

			glBindTexture(iTextureTarget, 0);

			iColorBuffer = 0;
		} else {
			bMipmap = false;
			int maxSamples = glGetInteger(GL_MAX_SAMPLES);

			iSamples = MathTools.clip(0, iSamples, maxSamples);

			iColorBuffer = glGenRenderbuffers();
			glBindRenderbuffer(GL_RENDERBUFFER, iColorBuffer);

			glRenderbufferStorageMultisample(GL_RENDERBUFFER,
					iSamples, 0, iWidth, iHeight);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, iColorBuffer);

		iSamples = glGetRenderbufferParameteri(GL_RENDERBUFFER,
				GL_RENDERBUFFER_SAMPLES);

		m_Format.setTextureTarget(iTextureTarget);
		m_Format.setSamples(iSamples);
		m_Format.setMipmap(bMipmap);
		m_Format.setAttachment(eAttachment);

		glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);

	}

	GLFramebufferObjectFormat format() {
		return m_Format;
	}

	bool isValid() {
		return m_iFBO > 0;
	}

	bool isBound() {
		return GLContext.getCurrentContext().getCurrentFBO() == m_iFBO;
	}

	bool bind() {

		if (m_iFBO > 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_iFBO);
			GLContext.getCurrentContext().setCurrentFBO(m_iFBO);
			return true;
		}

		return false;
	}

	void release() {
		if (GLContext.getCurrentContext().getCurrentFBO() == m_iFBO)
		{
			GLContext.getCurrentContext().setCurrentFBO(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	int width() {
		return m_iWidth;
	}

	int height() {
		return m_iHeight;
	}

	int texture() {
		return m_Format.getTextureTarget();
	}

	BufferedImage toImage() {
		// TODO
		return null;
	}

	EnumAttachment attachment() {
		return m_Format.getAttachment();
	}

	void setAttachment(EnumAttachment eAttachment) {
		m_Format.setAttachment(eAttachment);
	}

	GLFramebufferObjectFormat m_Format = new GLFramebufferObjectFormat();
	int m_iWidth;
	int m_iHeight;
	int m_iTarget;
	int m_iFBO = 0;
};
