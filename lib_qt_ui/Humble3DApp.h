#ifndef HUMBLE3DAPP_H
#define HUMBLE3DAPP_H

// Lib
#include "lib_qt_ui_global.h"

// Core
class CSceneManager;
class CAnimationManager;
class CCamera;

// Ui
#include "AView.h"
class CWidget3D;
class CGraphicsView3D;
class CSoftwareView3D;

// Qt
class QWidget;


class LIB_QT_UI_SHARED_EXPORT Humble3DApp
{
public:

    //! Constructeur
    Humble3DApp();

    //! Destructeur
    virtual ~Humble3DApp();

    //! Instance unique de l'application
    static Humble3DApp* instance();

    //! Retourne un pointeur sur le gestionnaire de scene 3D
    CSceneManager* getSceneManager();

    //! Retourne un pointeur sur le gestionnaire d'animations
    CAnimationManager* getAnimationManager();

    //! Crée une vue 3D simple
    CWidget3D* createWidget3D(CCamera* pCamera, QWidget* parent = 0);

    //! Crée une vue 3D basée sur une QGraphicsView
    CGraphicsView3D* createGraphicsView3D(CCamera* pCamera, QWidget* parent = 0);

    //! Crée une vue 3D avec un rendu software primitif
    CSoftwareView3D* createSoftwareView3D(CCamera* pCamera, QWidget* parent = 0);

    //! Retourne la liste des vues
    const QList<AView*>& getViews() const;

    //! Retourne la liste des vues
    QList<AView*>& getViews();

protected:

    static Humble3DApp* s_instance;

    //! Pointeur sur le gestionnaire de scene 3D
    CSceneManager* m_pSceneManager;

    //! Pointeur sur le gestionnaire d'animations
    CAnimationManager* m_pAnimationManager;

    //! Vues
    QList<AView*> m_Views;
};

#endif // HUMBLE3DAPP_H
