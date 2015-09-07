#ifndef CMOUSESTATES_H
#define CMOUSESTATES_H

class CMouseStates
{
public:
	CMouseStates()
		: m_bRightBtnPressed(false)
		, m_bMidBtnPressed(false)
		, m_bLeftBtnPressed(false)
	{}
	

	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! D�finit la position du pointeur
	void setPosition(const QPoint& position) { m_Position = position; }

	//! D�finit la position globale du pointeur
	void setGlobalPosition(const QPoint& globalPosition) { m_GlobalPosition = globalPosition; }

	//! D�finit si le button droit est press�
	void setRightButtonPressed(bool bPressed) { m_bRightBtnPressed = bPressed; }

	//! D�finit si le button central est press�
	void setMidButtonPressed(bool bPressed) { m_bMidBtnPressed = bPressed; }

	//! D�finit si le button gauche est press�
	void setLeftButtonPressed(bool bPressed) { m_bLeftBtnPressed = bPressed; }

	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne la position du pointeur
	QPoint getPosition() const { return m_Position; }

	//! Retourne la position globale du pointeur
	QPoint getGlobalPosition() const { return m_GlobalPosition; }

	//! Retourne vrai si le button droit est press�
	bool isRightButtonPressed() const { return m_bRightBtnPressed; }

	//! Retourne vrai si le button central est press�
	bool isMidButtonPressed() const { return m_bMidBtnPressed; }

	//! Retourne vrai si le button gauche est press�
	bool isLeftButtonPressed() const { return m_bLeftBtnPressed; }

private:
	
	QPoint m_Position;
	QPoint m_GlobalPosition;
	bool m_bRightBtnPressed;
	bool m_bMidBtnPressed;
	bool m_bLeftBtnPressed;
};

#endif // CMOUSESTATES_H
