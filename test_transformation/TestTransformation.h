#ifndef TESTTRANSFORMATION_H
#define TESTTRANSFORMATION_H

#include "Humble3DApp.h"

class TestTransformation : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    TestTransformation();

    virtual ~TestTransformation();

private slots:

    void onMousePressed();
    void onMouseMoved();
    void onMouseReleased();
    void onTouchScaleStarted();
    void onTouchScaleChanged(real dScaleFactor);
    void onTouchScaleEnded();

private:

    CWidget3D* m_pView;

    void wheel(int delta);
    void rotate(int deltax, int deltay);

    CSceneNode* m_pSunNode;

    CSceneNode* m_pSunToEarthNode;
    CSceneNode* m_pSunToJupiterNode;

    CSceneNode* m_pEarthNode;
    CSceneNode* m_pJupiterNode;

    CSceneNode* m_pEarthToMoonNode;
    CSceneNode* m_pMoonNode;

    // Gestion de la camera
    bool m_bPressed;
    bool m_bTouch;
    QPoint m_StartPan;
    QPoint m_LastPan;
    QVector3D m_vStartEye;
    QVector3D m_vStartCenter;
    QVector3D m_vStartUpVector;
    QVector3D m_vNewEyePosition;

    QTime m_GlobalTime;


public slots:

    void onTimeout();
};

#endif // TESTTRANSFORMATION_H
