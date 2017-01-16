#ifndef HUMBLE3DQMLAPP_H
#define HUMBLE3DQMLAPP_H

// Lib
#include "lib_qml_ui_global.h"

// Core
class CSceneManager;
class CAnimationManager;
class CCamera;


class LIB_QML_UI_SHARED_EXPORT Humble3DQmlApp
{
public:

    //! Constructeur
    Humble3DQmlApp();

    //! Destructeur
    virtual ~Humble3DQmlApp();

    //! Instance unique de l'application
    static Humble3DQmlApp* instance();

    //! Retourne un pointeur sur le gestionnaire de scene 3D
    CSceneManager* getSceneManager();

    //! Retourne un pointeur sur le gestionnaire d'animations
    CAnimationManager* getAnimationManager();

protected:

    static Humble3DQmlApp* s_instance;

    //! Pointeur sur le gestionnaire de scene 3D
    CSceneManager* m_pSceneManager;

    //! Pointeur sur le gestionnaire d'animations
    CAnimationManager* m_pAnimationManager;
};

#endif // HUMBLE3DQMLAPP_H
