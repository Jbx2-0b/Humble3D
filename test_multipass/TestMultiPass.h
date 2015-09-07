#ifndef TEST_MULTIPASS_H
#define TEST_MULTIPASS_H

#include "Humble3DApp.h"

class TestMultiPass : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    //! Constructeur
    TestMultiPass();

    //! Destructeur
    virtual ~TestMultiPass();

protected slots:

    void onFPSChanged(int iFPS);

protected:

    CWidget3D* m_pView;
    CCamera* m_pSceneCamera;
};

#endif // TEST_MULTIPASS_H
