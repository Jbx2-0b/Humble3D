#ifndef CBONE_H
#define CBONE_H

// Foundations
#include "CMath.h"
#include "CVertexWeight.h"

// Qt
#include <QList>
#include <QMatrix4x4>


class CBone
{
public:

	//! Constructeur
	CBone(const QString& nodeName = "", const QMatrix4x4& offsetMatrix = QMatrix4x4()) 
		: m_NodeName(nodeName)
		, m_OffsetMatrix(offsetMatrix) {}

    //! Définit le nom du noeud associé
    void setNodeName(const QString& nodeName) { m_NodeName = nodeName; }

    //! Retourne le nom du noeud associé
    const QString& getNodeName() const { return m_NodeName; }

	//! Retourne la liste des vertex influencés par ce bone
	QList<CVertexWeight>& vertexWeights() { return m_Weights; }

	//! Retourne la liste des vertex influencés par ce bone
	const QList<CVertexWeight>& vertexWeights() const { return m_Weights; }

	//! Retourne la matrice de transformation donnant l'offset par rapport au repos ( = emplacement des CSceneNode sans animation)
	const QMatrix4x4& getOffsetMatrix() const { return m_OffsetMatrix; }

	//! Définit la matrice de transformation donnant l'offset par rapport au repos ( = emplacement des CSceneNode sans animation)
	void setOffsetMatrix(const QMatrix4x4& offsetMatrix) { m_OffsetMatrix = offsetMatrix; }


private:

	//! Nom du noeud associé
	QString m_NodeName;

	//! The vertices affected by this bone
	QList<CVertexWeight> m_Weights;

	//! Matrix that transforms from mesh space to bone space in bind pose
	QMatrix4x4 m_OffsetMatrix;

};

#endif // CBONE_H
