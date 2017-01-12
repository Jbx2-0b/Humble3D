#ifndef TESTQML_H
#define TESTQML_H

#include "Humble3DApp.h"

class TestQml : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    TestQml();

    virtual ~TestQml();

private:
    CSceneNode* m_pSolarSystemNode;

    CSceneNode* m_pSunNode;

    CSceneNode* m_pSunToEarthNode;
    CSceneNode* m_pSunToJupiterNode;

    CSceneNode* m_pEarthNode;
    CSceneNode* m_pJupiterNode;

    CSceneNode* m_pEarthToMoonNode;
    CSceneNode* m_pMoonNode;

    // Gestion de la camera
    QVector3D m_vStartEye;
    QVector3D m_vStartCenter;
    QVector3D m_vStartUpVector;
    QVector3D m_vNewEyePosition;

    QTime m_GlobalTime;

public slots:
    void onTimeout();
};

#endif // TESTQML_H
