#ifndef CBLENDING_H
#define CBLENDING_H

// Qt
#include <QVector4D>

// Foundations
#include "CGeometryGlobal.h"

class CBlending
{
public:

	CBlending()
	{
		m_bEnabled = false;
		m_eSourceRGBFactor = Source::eOne;
		m_eSourceAlphaFactor = Source::eOne;
		m_eDestinationRGBFactor = Destination::eZero;
		m_eDestinationAlphaFactor = Destination::eZero;
		m_eRGBEquation = eAdd;
		m_eAlphaEquation = eAdd;
		m_Color = QVector4D(0, 0, 0, 0);
	}

	bool isEnabled() const { return m_bEnabled; }

	void setEnabled(bool val) { m_bEnabled = val; }

    void setBlendFunction(Source::EnumBlendingFactor eSourceFactor, Destination::EnumBlendingFactor eDestinationFactor)
    {
        m_eSourceRGBFactor = m_eSourceAlphaFactor = eSourceFactor;
        m_eDestinationRGBFactor = m_eDestinationAlphaFactor = eDestinationFactor;
    }

    void setBlendSeparateFunction(
            Source::EnumBlendingFactor eSourceRGBFactor,
            Destination::EnumBlendingFactor eDestinationRGBFactor,
            Source::EnumBlendingFactor eSourceAlphaFactor,
            Destination::EnumBlendingFactor eDestinationAlphaFactor)
    {
        m_eSourceRGBFactor = eSourceRGBFactor;
        m_eDestinationRGBFactor = eDestinationRGBFactor;
        m_eSourceAlphaFactor = eSourceAlphaFactor;
        m_eDestinationAlphaFactor = eDestinationAlphaFactor;
    }

    void setBlendEquation(EnumBlendEquation eRgbEquation, EnumBlendEquation eAlphaEquation)
    {
        m_eRGBEquation = eRgbEquation;
        m_eAlphaEquation = eAlphaEquation;

    }

	Source::EnumBlendingFactor getSourceRGBFactor() const { return m_eSourceRGBFactor; }

	void setSourceRGBFactor(Source::EnumBlendingFactor val) { m_eSourceRGBFactor = val; }

	Source::EnumBlendingFactor getSourceAlphaFactor() const { return m_eSourceAlphaFactor; }

	void setSourceAlphaFactor(Source::EnumBlendingFactor val) { m_eSourceAlphaFactor = val; }

	Destination::EnumBlendingFactor getDestinationRGBFactor() const { return m_eDestinationRGBFactor; }

	void setDestinationRGBFactor(Destination::EnumBlendingFactor val) { m_eDestinationRGBFactor = val; }

	Destination::EnumBlendingFactor getDestinationAlphaFactor() const { return m_eDestinationAlphaFactor; }

	void setDestinationAlphaFactor(Destination::EnumBlendingFactor val) { m_eDestinationAlphaFactor = val; }

	EnumBlendEquation getRGBEquation() const { return m_eRGBEquation; }

	void setRGBEquation(EnumBlendEquation val) { m_eRGBEquation = val; }

	EnumBlendEquation getAlphaEquation() const { return m_eAlphaEquation; }

	void setAlphaEquation(EnumBlendEquation val) { m_eAlphaEquation = val; }

	QVector4D getColor() const { return m_Color; }

	void setColor(const QVector4D& val) { m_Color = val; }

	friend bool operator != (const CBlending& b1, const CBlending& b2);

private:

	bool m_bEnabled;
	Source::EnumBlendingFactor m_eSourceRGBFactor;
	Source::EnumBlendingFactor m_eSourceAlphaFactor;
	Destination::EnumBlendingFactor m_eDestinationRGBFactor;
	Destination::EnumBlendingFactor m_eDestinationAlphaFactor;
	EnumBlendEquation m_eRGBEquation;
	EnumBlendEquation m_eAlphaEquation;
	QVector4D m_Color;
};


inline bool operator != (const CBlending& b1, const CBlending& b2)
{
	return 
		b1.m_bEnabled != b2.m_bEnabled || 
		b1.m_eSourceRGBFactor != b2.m_eSourceRGBFactor ||
		b1.m_eSourceAlphaFactor != b2.m_eSourceAlphaFactor ||
		b1.m_eDestinationRGBFactor != b2.m_eDestinationRGBFactor ||
		b1.m_eDestinationAlphaFactor != b2.m_eDestinationAlphaFactor ||
		b1.m_eRGBEquation != b2.m_eRGBEquation ||
		b1.m_eAlphaEquation != b2.m_eAlphaEquation ||
		b1.m_Color != b2.m_Color;
}
#endif // CBLENDING_H
