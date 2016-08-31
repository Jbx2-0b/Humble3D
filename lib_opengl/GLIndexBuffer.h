package opengl.buffers;

import opengl.EnumBufferType;
import geometry.buffers.AIndexBuffer;
import geometry.renderstates.EnumUsagePattern;

class GLIndexBuffer extends AIndexBuffer {

	GLIndexBuffer(EnumUsagePattern usagePattern) {
		super(usagePattern);
		
		buffer.create();
		buffer.setUsagePattern(usagePattern);
	}

	//! Override
	<T> void allocate(T bufferInSystemMemory) {
		buffer.allocate(bufferInSystemMemory);
	}
	
	//! Override
	<T> void copyToSystemMemory(T bufferInSystemMemory) {
		buffer.read(0, bufferInSystemMemory);
		
	}

	//! Override
	<T> void copyFromSystemMemory(T bufferInSystemMemory) {
		buffer.write(0, bufferInSystemMemory);
		
	}

	//! Override
	<T> void copyToSystemMemory(T bufferInSystemMemory,
			int destinationOffsetInBytes) {
		buffer.read(destinationOffsetInBytes, bufferInSystemMemory);
		
	}

	//! Override
	<T> void copyFromSystemMemory(T bufferInSystemMemory,
			int destinationOffsetInBytes) {
		buffer.write(destinationOffsetInBytes, bufferInSystemMemory);
		
	}

	GLBuffer buffer = new GLBuffer(EnumBufferType.eIndexBuffer);

}
