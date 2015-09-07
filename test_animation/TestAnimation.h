#ifndef TEST_ANIMATION_H
#define TEST_ANIMATION_H

// #define SOFTWARE_RENDERING

#include "Humble3DApp.h"

class TestAnimation : public QObject, public Humble3DApp
{
	Q_OBJECT

public:

	//! Constructeur
    TestAnimation();

	//! Destructeur
	virtual ~TestAnimation();

protected:

#ifdef SOFTWARE_RENDERING
    CSoftwareView3D* m_pView;
#else
     CWidget3D* m_pView;
#endif

	bool m_bKeyUpPressed;
	bool m_bKeyDownPressed;
	bool m_bKeyRightPressed;
	bool m_bKeyLeftPressed;

	CCamera* m_pCamera;

};

#endif // TEST_ANIMATION_H
