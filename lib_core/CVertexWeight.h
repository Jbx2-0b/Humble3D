#ifndef CVERETXWEIGHT_H
#define CVERETXWEIGHT_H

// Foundations
#include "CMath.h"

class CVertexWeight
{
public:

	CVertexWeight()
		: m_iVertexID(0)
		, m_dWeight(0.)
	{}

	CVertexWeight(unsigned int iVertexID, real dWeight)
		: m_iVertexID(iVertexID)
		, m_dWeight(dWeight)
	{}

	//! Définit l'index du vertex influencé par le bone
	void setVertexID(unsigned int iVertexID) { m_iVertexID = iVertexID; }

	//! Définit la force de l'influence dans l'intervalle [0; 1]
	void setWeight(real dWeight) { m_dWeight = dWeight; }

	//! Retourne l'index du vertex influencé par le bone
	unsigned int getVertexID() const { return m_iVertexID; }

	//! Retourne la force de l'influence dans l'intervalle [0; 1]
	real getWeight() const { return m_dWeight; }

private:
	//! Index of the vertex which is influenced by the bone.
	unsigned int m_iVertexID;

	//! The strength of the influence in the range (0...1).
	//! The influence from all bones at one vertex amounts to 1.
	real m_dWeight;
};

#endif // CVERETXWEIGHT_H