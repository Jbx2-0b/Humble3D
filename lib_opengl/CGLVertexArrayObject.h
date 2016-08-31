#ifndef CGLVERTEXARRAYOBJECT_H
#define CGLVERTEXARRAYOBJECT_H

#include "CGLGlobal.h"

/*
class QOpenGLVertexArrayObject : protected CGLFunctions
{
public:
    QOpenGLVertexArrayObject()
		: m_iVAO(0)
	{
        initializeOpenGLFunctions();
	}

    ~QOpenGLVertexArrayObject()
	{
		destroy();
	}

	bool create()
	{
		if (m_iVAO > 0)
			return false; // Already created

		glGenVertexArrays(0, &m_iVAO);

		return true;
	}

	void destroy()
	{
		if (m_iVAO > 0)
		{
			glDeleteVertexArrays(0, &m_iVAO);
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

private:

	int m_iVAO;
};
*/

#endif // CGLVERTEXARRAYOBJECT_H
