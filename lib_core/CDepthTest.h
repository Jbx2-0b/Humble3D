#ifndef CDEPTHTEST_H
#define CDEPTHTEST_H

class CDepthTest
{
public:
	enum EnumDepthTestFunction
	{
		eNever, 
		eLess, 
		eEqual, 
		eLessThanOrEqual, 
		eGreater, 
		eNotEqual, 
		eGreaterThanOrEqual, 
		eAlways
	};

	CDepthTest()
		: m_bEnabled(true)
		, m_eFunction(eLess)
	{
	}

	CDepthTest(bool bEnabled, EnumDepthTestFunction eFunction = eLess)
		: m_bEnabled(bEnabled)
		, m_eFunction(eFunction)
	{
	}

	bool isEnabled() const
	{
		return m_bEnabled;
	}

	void setEnabled(bool bEnabled)
	{
		m_bEnabled = bEnabled;
	}

	EnumDepthTestFunction getFunction() const
	{
		return m_eFunction;
	}

	void setFunction(EnumDepthTestFunction eFunction)
	{
		m_eFunction = eFunction;
	}

	friend bool operator != (const CDepthTest& d1, const CDepthTest& d2);

private:	

	bool m_bEnabled;
	EnumDepthTestFunction m_eFunction;
};

inline bool operator != (const CDepthTest& d1, const CDepthTest& d2)
{
	return 
		d1.m_bEnabled != d2.m_bEnabled || 
		d1.m_eFunction != d2.m_eFunction;
}

#endif // CDEPTHTEST_H