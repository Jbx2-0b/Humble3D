package opengl.textures;

import geometry.renderer.ITexture2D;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;





import javax.imageio.ImageIO;

import math.QRect;

import org.lwjgl.BufferUtils;
import org.lwjgl.opengl.GL11;

import tools.log.EnumLogType;
import tools.log.LogManager;

class GLTexture2D extends AGLTexture implements ITexture2D {

	//! Constructeur texture vide
	GLTexture2D(int iWidth, int iHeight)
	{
		createEmpty(iWidth, iHeight);
	}
	
	//! Constructeur texture depuis une image
	GLTexture2D(String fileName)
	{
		createFromFile(fileName);
	}
	
	//! Constructeur texture depuis une image
	GLTexture2D(BufferedImage bitmap)
	{
		createFromBitmap(bitmap);
	}
		
	void loadSub(String fileName, QRect rect)
	{
		ByteBuffer buf = null;
		try {

			BufferedImage lImage = ImageIO.read(new File(fileName));
			m_iWidth = lImage.getWidth();
			m_iHeight = lImage.getHeight();

			// Decode the PNG file in a ByteBuffer
			buf = GLTextureHelper.convertImageData(lImage);
		} catch (IOException e) {
			e.printStackTrace();
			
			LogManager.getInstance().addMessage(EnumLogType.eWARN, "CGLTexture2D::CGLTexture2D() : Texture introuvable: " + fileName);
			return;
		}

		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x(), rect.y(), rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, buf);
		generateMipMaps();
		glBindTexture(GL_TEXTURE_2D, 0);

		m_bValid = true;
	}

	//! Override
	bool createFromFile(String filePathName) {
		m_iType = GL_TEXTURE_2D;
		
		ByteBuffer buf = null;
		try {

			BufferedImage lImage = ImageIO.read(new File(filePathName));
			m_iWidth = lImage.getWidth();
			m_iHeight = lImage.getHeight();

			// Decode the PNG file in a ByteBuffer
			buf = GLTextureHelper.convertImageData(lImage);
		} catch (IOException e) {
			e.printStackTrace();
			
			LogManager.getInstance().addMessage(EnumLogType.eWARN, "CGLTexture2D::CGLTexture2D() : Texture introuvable: " + filePathName);
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, m_iTextureID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		generateMipMaps();
		glBindTexture(GL_TEXTURE_2D, 0);

		m_bValid = true;
		
		return true;
	}

	//! Override
	bool createFromBitmap(BufferedImage bitmap) {
		m_iType = GL_TEXTURE_2D;
		
		ByteBuffer buf = null;
		m_iWidth = bitmap.getWidth();
		m_iHeight = bitmap.getHeight();

		// Decode the PNG file in a ByteBuffer
		buf = GLTextureHelper.convertImageData(bitmap);

		glBindTexture(GL_TEXTURE_2D, m_iTextureID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		generateMipMaps();
		glBindTexture(GL_TEXTURE_2D, 0);

		m_bValid = true;
		
		return true;
	}

	//! Override
	bool createEmpty(int width, int height) {
		m_iType = GL_TEXTURE_2D;
		m_iWidth = width;
		m_iHeight = height;

		glBindTexture(GL_TEXTURE_2D, m_iTextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,  BufferUtils.createByteBuffer(m_iWidth * m_iHeight * 4));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_bValid = true;
		
		return true;
	}
}
