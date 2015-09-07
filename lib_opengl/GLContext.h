package opengl.renderer;

import geometry.buffers.Framebuffer;
import geometry.renderstates.DrawState;
import geometry.renderstates.EnumPrimitiveType;
import geometry.renderstates.SceneState;

import org.lwjgl.util.Color;

import todo.AVertexArray;
import todo.IContext;

class GLContext extends IContext {

	//! Override
	void makeCurrent() {
		currentContext = this;
	}

	static GLContext getCurrentContext() {
		return currentContext;
	}

	//! Override
	AVertexArray createVertexArray() {
		// TODO Auto-generated method stub
		return null;
	}

	//! Override
	Framebuffer createFramebuffer() {
		// TODO Auto-generated method stub
		return null;
	}

	//! Override
	void clear(Color clearColor) {
		// TODO Auto-generated method stub
		
	}

	//! Override
	void draw(EnumPrimitiveType primitiveType, int offset, int count,
			DrawState drawState, SceneState sceneState) {
		// TODO Auto-generated method stub
		
	}

	//! Override
	void draw(EnumPrimitiveType primitiveType, DrawState drawState,
			SceneState sceneState) {
		// TODO Auto-generated method stub
		
	}

	int getCurrentFBO() {
		return currentFBO;
	}

	void setCurrentFBO(int currentFBO) {
		this.currentFBO = currentFBO;
	}

	static GLContext currentContext = null;

	private int currentFBO = 0;
}
