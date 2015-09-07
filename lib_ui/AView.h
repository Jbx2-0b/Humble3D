#ifndef AVIEW_H
#define AVIEW_H


#include "ARenderer.h"
#include "CFPSManager.h"
#include "CMouseStates.h"
#include "CKeyboardStates.h"
#include "CFilterEvent.h"


class CSceneManager;

class AView : public CFilterEvent::IFilterEventListener
{
public:

	//! Constructeur
    AView(CSceneManager* pSceneManager);

	//! Destructeur
    virtual ~AView() = 0;

    //! Retourne un pointeur sur le renderer
    ARenderer* getRenderer() { return m_pRenderer; }

    //! Retourne un pointeur sur le gestionnaire de scene 3D
    CSceneManager* getSceneManager() { return m_pSceneManager; }

	//! Definit la couleur de fond
    virtual void setBackgroundColor(const QVector4D& color) { m_pRenderer->setBackgroundColor(color); }

    //! Retourne la couleur de fond
    QColor getBackgroundColor() const { return m_BackgroundColor; }

	//! Accesseur sur le gestionnaire de FPS
    CFPSManager* getFPSManager() { return &m_FPSManager; }

	//! Retourne l'etat de la souris
	CMouseStates getMouseStates() const { return m_MouseStates; }

	//! Retourne l'etat du clavier
	CKeyboardStates getKeyboardStates() const { return m_KeyboardStates; }

    //! Définit la caméra utilisée
    void setCurrentCamera(CCamera* pCamera) { m_pRenderer->setCurrentCamera(pCamera); }

    //! Retourne la caméra utilisée
    CCamera* getCurrentCamera() { return m_pRenderer->getCurrentCamera(); }

    //! Retourne la caméra utilisée
    const CCamera* getCurrentCamera() const { return m_pRenderer->getCurrentCamera(); }

protected:

    //! Renderer OpenGL
    ARenderer* m_pRenderer;

    //! Pointeur sur le gestionnaire de scene 3D
    CSceneManager* m_pSceneManager;

	//! Couleur de fond
	QColor m_BackgroundColor;

	//! Gestionnaire de FPS
    CFPSManager m_FPSManager;

	//! Etat de la souris
	CMouseStates m_MouseStates;

	//! Etat du clavier
	CKeyboardStates m_KeyboardStates;

    //! Filtreur d'evenement
    CFilterEvent m_MouseMoveFilter;

private:

    //! Copy constructor
    AView(const AView&);
};

#endif // AVIEW_H
