#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CWidget3D.h"

// Qt
#include <QLabel>

class Controller : public QObject
{
	Q_OBJECT

public:

    Controller(CWidget3D* pView);

    virtual ~Controller();

private slots:

	void onMousePressed();
	void onMouseMoved();
	void onMouseReleased();
	void onTouchScaleStarted();
	void onTouchScaleChanged(real dScaleFactor);
	void onTouchScaleEnded();
    void onKeyPressed();
    void onFPSChanged(int iFPS);

private:

	void wheel(int delta);
	void rotate(int deltax, int deltay);

    CWidget3D* m_pView;
    QLabel* m_pLabel;

	// Gestion de la camera
	bool m_bPressed;
	bool m_bTouch;
	QPoint m_StartPan;
	QPoint m_LastPan;
	QVector3D m_vStartEye;
	QVector3D m_vStartCenter;
	QVector3D m_vStartUpVector;

};

#endif // CONTROLLER_H
