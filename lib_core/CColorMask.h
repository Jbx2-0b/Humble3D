#ifndef CCOLORMASK_H
#define CCOLORMASK_H

class CColorMask
{
public:

	CColorMask()
		: m_bRed(true)
		, m_bGreen(true)
		, m_bBlue(true)
		, m_bAlpha(true)
	{
	}

	CColorMask(bool bRed, bool bGreen, bool bBlue, bool bAlpha)
	{
		m_bRed = bRed;
		m_bGreen = bGreen;
		m_bBlue = bBlue;
		m_bAlpha = bAlpha;
	}

    bool isRedEnabled() const
	{
		return m_bRed;
	}

    void setRedEnabled(bool bRed)
	{
		m_bRed = bRed;
	}

    bool isGreenEnabled() const
	{
		return m_bGreen;
	}

    void setGreenEnabled(bool bGreen)
	{
		m_bGreen = bGreen;
	}

    bool isBlueEnabled() const
	{
		return m_bBlue;
	}

    void setBlueEnabled(bool bBlue)
	{
		m_bBlue = bBlue;
	}

    bool isAlphaEnabled() const
	{
		return m_bAlpha;
	}

    void setAlphaEnabled(bool bAlpha)
	{
		m_bAlpha = bAlpha;
	}

	friend bool operator != (const CColorMask& c1, const CColorMask& c2);

private:
	bool m_bRed;
	bool m_bGreen;
	bool m_bBlue;
	bool m_bAlpha;
};

inline bool operator != (const CColorMask& c1, const CColorMask& c2)
{
	return 
		c1.m_bRed	!= c2.m_bRed || 
		c1.m_bGreen != c2.m_bGreen ||
		c1.m_bBlue	!= c2.m_bBlue ||
		c1.m_bAlpha != c2.m_bAlpha;
}

#endif // CCOLORMASK_H
