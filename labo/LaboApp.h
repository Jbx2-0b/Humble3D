#ifndef LABOAPP_H
#define LABOAPP_H

#include "Humble3DApp.h"
#include "CGLRenderer.h"
#include "CGraphicsWidgetItem.h"
#include "CToolBar.h"
#include "CExclusiveButton.h"
#include "CEditShaderTextItem.h"
#include "CDebugManager.h"

class CAnimatedPanelDecorator;
class CNodeTreeWidget;
class CToolBar;

#include <QProgressBar>
#include <QSettings>

#ifdef MULTITHREAD
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#endif

static const QString configFileName = "config.ini";


static const int iDefaultViewSizeX = 800;
static const int iDefaultViewSizeY = 475;
static const int iDefaultViewPosX = 2000;
static const int iDefaultViewPosY = 100;

class LaboApp : public QObject, public Humble3DApp
{
    Q_OBJECT

public:

    //! Différentes options
    enum EnumOptions
    {
        eOptionZoom,
        eOptionRotate,
        eOptionPan,
        eOptionFOV,
    };

    //! Constructeur
    LaboApp();

    //! Destructeur
    virtual ~LaboApp();

    //! Charge une scene
    void loadScene(const QString& fileName);

protected slots:

    void onMousePressed();
    void onMouseMoved();
    void onMouseReleased();
    void onKeyPressed();
    void onViewSizeChanged(const QSize& size);
    void onLoad();
    void onLoadFinished();
    void onFullScreen();
    void onRunEditSwitch();
    void onOptionChanged();
    void onEditShadersClicked();
    void onEditShader(const QString& name);
    void onPlayAnimation(const QString& name, bool bPlay);
    void onTreeWidgetOpen();
    void onTreeWidgetClosing();
    void onTimeout();
    void onVSButtonClicked();
    void onGSButtonClicked();
    void onFSButtonClicked();
    void onFPSChanged(int iFPS);

    //virtual void keyPressEvent(QKeyEvent* pEvent);

protected:

    static const int iBorderEditItem = 2;
    static const int iTreeWidgetWidth = 300;

    void wheel(int delta);
    void pan(int deltax, int deltay);
    void rotate(int deltax, int deltay);
    void fov(int delta);

    CGraphicsView3D* m_pView;

    QSettings m_Settings;

    //! Item textuel qui permet d'éditer les shaders
    CEditShaderTextItem* m_pEditShadersTextItem;

    //! Arbre des noeuds et paramétres de scéne
    CNodeTreeWidget* m_pTreeWidget;

    //! Decorateur de l'arbre
    CAnimatedPanelDecorator* m_pTreeWidgetDecorator;

    //! Barre d'outils supérieure
    CToolBar* m_pTopToolBar;

    //! Barre d'outils inférieure
    CToolBar* m_pBottomToolBar;

#ifdef MULTITHREAD
    //! Permet de suivre le thread de chargement de la scene
    QFutureWatcher<void> m_FutureWatcher;
#endif // MULTITHREAD

    //! Bouton plein écran
    QPushButton* m_pRunButton;
    QPushButton* m_pTreeButton;
    QPushButton* m_pFullScreenButton;
    QPushButton* m_pLoadButton;
    QPushButton* m_pSaveButton;
    QPushButton* m_pEditShadersButton;
    QPushButton* m_pEditVSButton;
    QPushButton* m_pEditGSButton;
    QPushButton* m_pEditFSButton;
    QPushButton* m_pSaveShaderButton;
    QPushButton* m_pEditItemButton;
    QPushButton* m_pZoomButton;
    QPushButton* m_pRotateButton;
    QPushButton* m_pPanButton;
    QPushButton* m_pFOVButton;
    QPushButton* m_pResetButton;
    QPushButton* m_pAxeXZButton;
    QPushButton* m_pAxeYButton;

    QLabel* m_pFPSLabel;

    QProgressBar* m_pLoadProgressBar;

    //! Option en cours
    EnumOptions m_eCurrentOption;

    //! Gére l'exclusivité entre les boutons d'option
    CExclusiveButton m_ExclusiveOptionButton;

    //! Gére l'exclusivité entre les boutons d'édition des shaders
    CExclusiveButton m_ExclusiveEditShadersButton;

    //! Vrai si plein écran
    bool m_bFullScreen;

    //! Vrai si en mode edition
    bool m_bRun;

    // Gestion de la camera
    bool m_bPressed;
    QPoint m_LastPan;
    QPoint m_StartPan;
    QVector3D m_vStartEye;
    QVector3D m_vStartCenter;
    QVector3D m_vStartUpVector;
    real m_vStartFOV;

    CSceneNode* m_pCenterNode;

    CMeshInstance* m_pSelectedMeshInstance;
    CMeshInstance* m_pSelectedBoundingBox;
    CDebugManager* m_pDebugManager;

    bool m_bMergeMaterials;
    bool m_bRescaleMeshs;
    bool m_bInvertXZ;

    bool m_bIsEditingShaders;

    CShader* m_pSDEdited;

    QSet<AEntity*> m_AutoCreatedEntities;
};

#endif // LABOAPP_H
