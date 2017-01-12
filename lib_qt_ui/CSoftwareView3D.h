#ifndef CSOFTWAREVIEW_H
#define CSOFTWAREVIEW_H

// Lib
#include "lib_qt_ui_global.h"

// Foundations
#include "AView.h"

class CSceneManager;
class CSoftwareRenderer;

// Qt
#include <QWidget>
#include <QTimer>



class LIB_QT_UI_SHARED_EXPORT CSoftwareView3D : public QWidget, public AView
{
    Q_OBJECT

public:
    CSoftwareView3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget *parent = 0);

    virtual ~CSoftwareView3D();

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

protected:

    virtual void paintEvent(QPaintEvent*);

    virtual void resizeEvent(QResizeEvent* pEvent);

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

    void onUpdate(CFilterEvent*);

protected slots:

    void onTimeOut();

private:

    CSoftwareRenderer* m_pSoftwareRenderer;
};

#endif // CSOFTWAREVIEW_H
