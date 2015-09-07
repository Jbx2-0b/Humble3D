#ifndef CGRAPHICSVIEW3D_H
#define CGRAPHICSVIEW3D_H

// Lib
#include "lib_ui_global.h"

// Foundations
#include "CMath.h"
#include "CGraphicsScene3D.h"
#include "CToolBar.h"
#include "AView.h"

class CGLRenderer;
class CSceneManager;

//Qt
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QTimer>

// Qt
#include <QApplication>

class CGraphicsWidgetItem;

class LIB_UI_SHARED_EXPORT CGraphicsView3D
        : public QGraphicsView
        , public AView
{
    Q_OBJECT

public:

    //! Ctor - crée une application OpenGL basée sur une QGraphicsView
    CGraphicsView3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget* pParent = 0);

    //! Dtor
    virtual ~CGraphicsView3D();

    //! Retourne un pointeur sur la scene 2D
    CGraphicsScene3D* getGraphicsScene()	{ return m_pGraphicsScene; }

    //! Retourne true si la souris est sur un widget
    bool isOnWidget(const QPoint& point);

    //! Retourne le format d'affichage
    CGLFormat getFormat() const { return m_pGLWidget->format(); }

    //! Créer une barre d'outils
    CToolBar* createToolBar(CToolBar::EnumToolBarPosition eToolBarPosition = CToolBar::eToolBarTop);

    //! Supprime un objet de la barre d'outils
    void removeWidget(QWidget* pWidget);

    //! Retourne un pointeur sur le renderer OpenGL
    CGLRenderer* getGLRenderer() { return m_pGLRenderer; }

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

    //! Capture les evenements
    //virtual bool event(QEvent* pEvent);

    //! Capture les evenements de mouvement souris
    virtual void mouseMoveEvent(QMouseEvent* event);

    //! Capture les evenements de clique souris
    virtual void mousePressEvent(QMouseEvent* event);

    //! Capture les evenements de relachement souris
    virtual void mouseReleaseEvent(QMouseEvent* event);

    //! Capture les evenements de double clique souris
    virtual void mouseDoubleClickEvent(QMouseEvent* event);

    //! Capture les evenements de roulette souris
    virtual void wheelEvent(QWheelEvent* event);

    //! Capture les evenements de redimensionnement
    virtual void resizeEvent(QResizeEvent* event);

    //! Capture la pression d'une touche
    virtual void keyPressEvent(QKeyEvent* event);

    //! Capture le relachement d'une touche
    virtual void keyReleaseEvent(QKeyEvent* event);

    //! Pointeur sur le widget OpenGL sur lequel est associée la vue
    CGLWidget* m_pGLWidget;

    //! Pointeur sur la scéne 2D
    CGraphicsScene3D* m_pGraphicsScene;

    //! Items graphiques placés sur la vue
    QMap<QWidget*, CGraphicsWidgetItem*> m_GraphicsWidgetItems;

    CGLRenderer* m_pGLRenderer;

    void onUpdate(CFilterEvent*);
};

#endif // CGRAPHICSVIEW3D_H
