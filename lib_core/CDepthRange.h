#ifndef CDEPTHRANGE_H
#define CDEPTHRANGE_H

class CDepthRange
{
public:
	CDepthRange()
	{
		m_dNear = 0.0;
		m_dFar = 1.0;
	}

	double getNear() const
	{
		return m_dNear;
	}

	void setNear(double dNear)
	{
		m_dNear = dNear;
	}

	double getFar() const
	{
		return m_dFar;
	}

	void setFar(double dFar)
	{
		m_dFar = dFar;
	}

	friend bool operator != (const CDepthRange& d1, const CDepthRange& d2);

private:
	double m_dNear;
	double m_dFar;
};


inline bool operator != (const CDepthRange& d1, const CDepthRange& d2)
{
	return 
		d1.m_dNear != d2.m_dNear || 
		d1.m_dFar != d2.m_dFar;
}

#endif // CDEPTHRANGE_H