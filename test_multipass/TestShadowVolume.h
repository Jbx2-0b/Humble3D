#ifndef TESTSHADOWVOLUME_H
#define TESTSHADOWVOLUME_H

#include "Humble3DApp.h"

class TestShadowVolume : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    //! Constructeur
    TestShadowVolume();

    //! Destructeur
    virtual ~TestShadowVolume();

protected slots:

    void onTimeout();

protected:

    CCamera* m_pSceneCamera;

    CSceneNode* m_pCenterNode;
};

#endif // TESTSHADOWVOLUME_H
