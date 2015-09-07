#ifndef CSTENCILTESTFACE_H
#define CSTENCILTESTFACE_H

class CStencilTestFace
{
public:

    CStencilTestFace(
            EnumStencilOperation eStencilFailOperation,
            EnumStencilOperation eDepthFailStencilPassOperation,
            EnumStencilOperation eDepthPassStencilPassOperation)
        : m_eStencilFailOperation(eStencilFailOperation)
        , m_eDepthFailStencilPassOperation(eDepthFailStencilPassOperation)
        , m_eDepthPassStencilPassOperation(eDepthPassStencilPassOperation)

    {
    }

	CStencilTestFace()
		: m_eStencilFailOperation(eKeep)
		, m_eDepthFailStencilPassOperation(eKeep)
        , m_eDepthPassStencilPassOperation(eKeep)
	{
	}

	EnumStencilOperation getStencilFailOperation() const
	{ 
		return m_eStencilFailOperation;
	}

	EnumStencilOperation getDepthFailStencilPassOperation() const
	{ 
		return m_eDepthFailStencilPassOperation;
	}

	EnumStencilOperation getDepthPassStencilPassOperation() const
	{ 
		return m_eDepthPassStencilPassOperation;
	}

	void setStencilFailOperation(EnumStencilOperation eOperation)
	{ 
		m_eStencilFailOperation = eOperation;
	}

	void setDepthFailStencilPassOperation(EnumStencilOperation eOperation)
	{ 
		m_eDepthFailStencilPassOperation = eOperation;
	}

	void setDepthPassStencilPassOperation(EnumStencilOperation eOperation)
	{ 
		m_eDepthPassStencilPassOperation = eOperation;
	}


	friend bool operator != (const CStencilTestFace& s1, const CStencilTestFace& s2);

private:

	EnumStencilOperation m_eStencilFailOperation;
	EnumStencilOperation m_eDepthFailStencilPassOperation;
	EnumStencilOperation m_eDepthPassStencilPassOperation;
	
};

inline bool operator != (const CStencilTestFace& s1, const CStencilTestFace& s2)
{
	return 
		s1.m_eStencilFailOperation			!= s2.m_eStencilFailOperation || 
		s1.m_eDepthFailStencilPassOperation	!= s2.m_eDepthFailStencilPassOperation ||
        s1.m_eDepthPassStencilPassOperation	!= s2.m_eDepthPassStencilPassOperation;

}


#endif // CSTENCILTESTFACE_H
