package opengl.buffers;

import org.lwjgl.opengl.GL30;

class GLVertexArrayObject {
	
	GLVertexArrayObject()
	{
		
	}
	
	void finalize()
	{
		destroy();
	}

	bool create()
    {
    	if (m_iVAO > 0)
    		return false; // Already created
    	
    	m_iVAO = glGenVertexArrays();
    	
    	return true;
    }
    
	void destroy()
    {
		if (m_iVAO > 0)
		{
			glDeleteVertexArrays(m_iVAO);
			m_iVAO = 0;
		}
    }
    
	bool isCreated()
    {
    	return m_iVAO > 0;
    }
    
	int objectId()
    {
    	return m_iVAO;
    }
    
	void bind()
    {
    	glBindVertexArray(m_iVAO);
    }
    
	void release()
    {
    	glBindVertexArray(0);
    }
    
    int m_iVAO = 0;
}
