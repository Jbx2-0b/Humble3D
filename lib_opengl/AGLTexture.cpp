#include "AGLTexture.h"
#include "CLogManager.h"

#include <QMatrix4x4>

static void convertToGLFormatHelper(QImage &dst, const QImage &img);

//-----------------------------------------------------------------------------------------
// AGLTEXTURE
//-----------------------------------------------------------------------------------------
AGLTexture::AGLTexture()
: m_iTextureID(0)
, m_bValid(false)
, m_iType(0)
, m_iWidth(0)
, m_iHeight(0)
{
    initializeOpenGLFunctions();
    glGenTextures(1, &m_iTextureID);
}

//-----------------------------------------------------------------------------------------
AGLTexture::~AGLTexture()
{
	if (m_iTextureID)
	{
		glDeleteTextures(1, &m_iTextureID);
		m_iTextureID = 0;
	}
}

//-----------------------------------------------------------------------------------------
void AGLTexture::generateMipMaps()
{
	glGenerateMipmap(m_iType);
}

//-----------------------------------------------------------------------------------------
void AGLTexture::bind()
{
	glBindTexture(m_iType, m_iTextureID);
	glEnable(m_iType);
}

//-----------------------------------------------------------------------------------------
void AGLTexture::release()
{
	glBindTexture(m_iType, 0);
	glDisable(m_iType);
}

//-----------------------------------------------------------------------------------------
QImage AGLTexture::convertToGLFormat(const QImage& img)
{
    QImage res(img.size(), QImage::Format_ARGB32);
    convertToGLFormatHelper(res, img.convertToFormat(QImage::Format_ARGB32));
    return res;
}

//-----------------------------------------------------------------------------------------
static void convertToGLFormatHelper(QImage &dst, const QImage &img)
{
    Q_ASSERT(dst.depth() == 32);
    Q_ASSERT(img.depth() == 32);

    if (dst.size() != img.size()) {
        int target_width = dst.width();
        int target_height = dst.height();
        qreal sx = target_width / qreal(img.width());
        qreal sy = target_height / qreal(img.height());

        quint32 *dest = (quint32 *) dst.scanLine(0); // NB! avoid detach here
        uchar *srcPixels = (uchar *) img.scanLine(img.height() - 1);
        int sbpl = img.bytesPerLine();
        int dbpl = dst.bytesPerLine();

        int ix = int(0x00010000 / sx);
        int iy = int(0x00010000 / sy);

        quint32 basex = int(0.5 * ix);
        quint32 srcy = int(0.5 * iy);

        // scale, swizzle and mirror in one loop
        while (target_height--) {
            const uint *src = (const quint32 *) (srcPixels - (srcy >> 16) * sbpl);
            int srcx = basex;
            for (int x=0; x<target_width; ++x) {
                uint src_pixel = src[srcx >> 16];

                if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                    dest[x] = (src_pixel << 8) | ((src_pixel >> 24) & 0xff);
                } else {
                    dest[x] = ((src_pixel << 16) & 0xff0000)
                              | ((src_pixel >> 16) & 0xff)
                              | (src_pixel & 0xff00ff00);
                }
                srcx += ix;
            }
            dest = (quint32 *)(((uchar *) dest) + dbpl);
            srcy += iy;
        }
    } else {
        const int width = img.width();
        const int height = img.height();
        const uint *p = (const uint*) img.scanLine(img.height() - 1);
        uint *q = (uint*) dst.scanLine(0);

        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
            for (int i=0; i < height; ++i) {
                const uint *end = p + width;
                while (p < end) {
                    *q = (*p << 8) | ((*p >> 24) & 0xff);
                    p++;
                    q++;
                }
                p -= 2 * width;
            }
        } else {
            for (int i=0; i < height; ++i) {
                const uint *end = p + width;
                while (p < end) {
                    *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);
                    p++;
                    q++;
                }
                p -= 2 * width;
            }
        }

    }
}
