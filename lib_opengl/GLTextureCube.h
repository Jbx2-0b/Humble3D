package opengl.textures;

import geometry.renderer.ITextureCube;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL12;
import org.lwjgl.opengl.GL13;

import tools.log.EnumLogType;
import tools.log.LogManager;

class GLTextureCube extends AGLTexture implements ITextureCube {

	GLTextureCube(List<String> fileNames)
	{
		createFromFiles(fileNames);
	}

	//! Override
	bool createFromFiles(List<String> fileNames) {
		m_iType = GL13.GL_TEXTURE_CUBE_MAP;

		List<ByteBuffer> images = new ArrayList<ByteBuffer>(); 

		for (int i = 0; i < 6; ++i)
		{
			try {
	
				BufferedImage lImage = ImageIO.read(new File(fileNames.get(i)));
				m_iWidth = lImage.getWidth();
				m_iHeight = lImage.getHeight();
	
				// Decode the PNG file in a ByteBuffer
				images.add(GLTextureHelper.convertImageData(lImage));
			} catch (IOException e) {
				e.printStackTrace();
				
				LogManager.getInstance().addMessage(EnumLogType.eWARN, "CGLTexture2D::CGLTexture2D() : Texture introuvable: " + fileNames.get(i));
				return false;
			}
		}

		glBindTexture(GL13.GL_TEXTURE_CUBE_MAP, m_iTextureID);

		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(0));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(1));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(2));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(3));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(4));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(5));

		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL12.GL_CLAMP_TO_EDGE);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL12.GL_CLAMP_TO_EDGE);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glBindTexture(GL13.GL_TEXTURE_CUBE_MAP, 0);

		m_bValid = true;
		
		return true;
	}

	//! Override
	bool createFromBitmap(List<BufferedImage> bitmaps) {
		m_iType = GL13.GL_TEXTURE_CUBE_MAP;

		List<ByteBuffer> images = new ArrayList<ByteBuffer>(); 

		m_iWidth = bitmaps.get(0).getWidth();
		m_iHeight = bitmaps.get(0).getHeight();
		
		for (int i = 0; i < 6; ++i)
		{
			// Decode the PNG file in a ByteBuffer
			images.add(GLTextureHelper.convertImageData(bitmaps.get(i)));
		}

		glBindTexture(GL13.GL_TEXTURE_CUBE_MAP, m_iTextureID);

		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(0));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(1));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(2));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(3));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(4));
		glTexImage2D(GL13.GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.get(5));

		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL12.GL_CLAMP_TO_EDGE);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL12.GL_CLAMP_TO_EDGE);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL13.GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glBindTexture(GL13.GL_TEXTURE_CUBE_MAP, 0);

		m_bValid = true;
		
		return true;
	}
}
