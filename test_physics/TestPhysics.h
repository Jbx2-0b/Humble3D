#ifndef TESTPHYSICS_H
#define TESTPHYSICS_H

#include "Humble3DApp.h"
#include "CPhysicsManager.h"

class TestPhysics : public QObject, public Humble3DApp
{
	Q_OBJECT

public:

    TestPhysics();

	virtual ~TestPhysics();

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

	// Gestion de la camera
	bool m_bPressed;
	bool m_bTouch;
	QPoint m_StartPan;
	QPoint m_LastPan;
	QVector3D m_vStartEye;
	QVector3D m_vStartCenter;
	QVector3D m_vStartUpVector;
    QVector3D m_vNewEyePosition;

    CPhysicsManager m_PhysicsManager;
};

#endif // TESTPHYSICS_H
