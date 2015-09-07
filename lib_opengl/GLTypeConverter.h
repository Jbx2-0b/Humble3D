package opengl;

import geometry.renderstates.EnumPrimitiveType;
import geometry.renderstates.EnumUsagePattern;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL15;
import org.lwjgl.opengl.GL21;
import org.lwjgl.opengl.GL32;

class GLTypeConverter {
	static int toGLType(EnumUsagePattern eUsagePattern)
	{
		switch (eUsagePattern)
		{
		case eStreamDraw:
		default:
			return GL15.GL_STREAM_DRAW;
		case eStreamRead:
			return GL15.GL_STREAM_READ;
		case eStreamCopy:
			return GL15.GL_STREAM_COPY;
		case eStaticDraw:
			return GL15.GL_STATIC_DRAW;
		case eStaticRead:
			return GL15.GL_STATIC_READ;
		case eStaticCopy:
			return GL15.GL_STATIC_COPY;
		case eDynamicDraw:
			return GL15.GL_DYNAMIC_DRAW;
		case eDynamicRead:
			return GL15.GL_DYNAMIC_READ;
		case eDynamicCopy:
			return GL15.GL_DYNAMIC_COPY;
		}
	}
	
	static int toGLType(EnumBufferType eBufferType)
	{
		switch (eBufferType)
		{
		case eVertexBuffer:
		default:
			return GL15.GL_ARRAY_BUFFER;
		case eIndexBuffer:
			return GL15.GL_ELEMENT_ARRAY_BUFFER;
		case ePixelPackBuffer:
			return GL_PIXEL_PACK_BUFFER;
		case ePixelUnpackBuffer:
			return GL_PIXEL_UNPACK_BUFFER;
		}
	}
	
	static int toGLType(EnumPrimitiveType ePrimitiveType)
	{
		switch (ePrimitiveType)
		{
		case ePrimitivePoint:
		default:
			return GL_POINTS;
		case ePrimitiveLine:
			return GL_LINES;
		case ePrimitiveLineLoop:
			return GL_LINE_LOOP;
		case ePrimitiveLineStrip:
			return GL_LINE_STRIP;
		case ePrimitiveTriangles:
			return GL_TRIANGLES;
		case ePrimitiveTriangleStrip:
			return GL_TRIANGLE_STRIP;
		case ePrimitiveTriangleFan:
			return GL_TRIANGLE_FAN;
		case ePrimitiveLinesAdjacency:
			return GL32.GL_LINES_ADJACENCY;
		case ePrimitiveLineStripAdjacency:
			return GL32.GL_LINE_STRIP_ADJACENCY;
		case ePrimitiveTrianglesAdjacency:
			return GL32.GL_TRIANGLES_ADJACENCY;
		case ePrimitiveTrianglesStripAdjacency:
			return GL32.GL_TRIANGLE_STRIP_ADJACENCY;
		}
	}
}
