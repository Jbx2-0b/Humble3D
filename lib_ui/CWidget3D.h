#ifndef CWIDGET3D_H
#define CWIDGET3D_H

// Lib
#include "lib_ui_global.h"

// Foundations
#include "CGLGlobal.h"
#include "CGLRenderer.h"
#include "AView.h"

class CSceneManager;
class CCamera;

// Qt
#include <QBasicTimer>


class LIB_UI_SHARED_EXPORT CWidget3D : public CGLWidget, public AView
{
    Q_OBJECT

public:

    //! Constructeur
    explicit CWidget3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget *parent = 0);

    //! Destructeur
    virtual ~CWidget3D();

	//! Retourne un pointeur sur le renderer OpenGL
    CGLRenderer* getGLRenderer() { return m_pGLRenderer; }

    //! Force la mise à jour du rendu
    void setForceUpdate(bool bForceUpdate);

signals:

	void mouseMoved();
	void mousePressed();
	void mouseReleased();
	void mouseDoubleClicked();
	void mouseWheelMoved();
	void keyPressed();
	void keyReleased();
	void sizeChanged(QSize size);
	void touchScaleStarted();
	void touchScaleChanged(real dScale);
	void touchScaleEnded();

protected slots:

	void onTimeOut();

protected:

    //! Capture les evenements
    virtual bool event(QEvent* pEvent);

    //! Capture les evenements de mouvement souris
    virtual void mouseMoveEvent(QMouseEvent* pEvent);

    //! Capture les evenements de clique souris
    virtual void mousePressEvent(QMouseEvent* pEvent);

    //! Capture les evenements de relachement souris
    virtual void mouseReleaseEvent(QMouseEvent* pEvent);

    //! Capture les evenements de double clique souris
    virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);

	void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

	CGLRenderer* m_pGLRenderer;

    bool m_bForceUpdate;

    void onUpdate(CFilterEvent* /*pFilter*/);
};

#endif // CWIDGET3D_H
