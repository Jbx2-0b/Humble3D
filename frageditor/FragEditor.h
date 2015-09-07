#ifndef FragEditor_H
#define FragEditor_H
#include "Humble3DApp.h"
#include "CGLRenderer.h"
#include "CGraphicsWidgetItem.h"
#include "CToolBar.h"
#include "CEditShaderTextItem.h"
#include "CImageSelectorItem.h"

class CToolBar;

#include <QProgressBar>

static const int iDefaultViewSizeX = 1200;
static const int iDefaultViewSizeY = 600;
static const int iDefaultViewPosX = 100;
static const int iDefaultViewPosY = 100;

class FragEditor : public QObject, public Humble3DApp//, public CGLRenderer::IGLRendererListener
{
	Q_OBJECT

public:

	//! Constructeur
    FragEditor();

	//! Destructeur
	virtual ~FragEditor();

protected:

    virtual void onNewMessage(bool bIsError, const QString& message);

protected slots:

	void onMousePressed();
	void onMouseMoved();
	void onMouseReleased();
	void onViewSizeChanged(const QSize& size);
	void onLoad();
	void onSave();
	void onFullScreen();
	void onEditShaderClicked();
	void onSelectTextureClicked();
	void onStopRenderingClicked();
	void onFPSChanged(int iFPS);
	void onSleepToolBarTimeout();

protected:

	static const int iBorderEditItem = 2;
	static const int iTreeWidgetWidth = 300;

    CGraphicsView3D* m_pView;

	//! Item textuel qui permet d'éditer les shaders
	CEditShaderTextItem* m_pEditShadersTextItem;

	//! Item qui permet de selectionner les images
	CImageSelectorItem* m_pImgSelectorItem;

	//! Barre d'outils supérieure
	CToolBar* m_pTopToolBar;

	//! Barre d'outils inférieure
	CToolBar* m_pBottomToolBar;

	//! Bouton plein écran
	QPushButton* m_pFullScreenButton;
	QPushButton* m_pLoadButton;
	QPushButton* m_pSaveButton;
	QPushButton* m_pEditShadersButton;
	QPushButton* m_pSelectTextureButton;
	QPushButton* m_pStopRenderingButton;

	QLabel* m_pFPSLabel;

	//! Vrai si plein écran
	bool m_bFullScreen;

	// Souris
	int m_iMouseX;
	int m_iMouseY;
	bool m_bMouseLeftPressed;
	bool m_bMouseRightPressed;
	
	// Gestion de la camera
	QPoint m_LastPan;
	QPoint m_StartPan;
	QVector3D m_vStartEye;
	QVector3D m_vStartCenter;
	QVector3D m_vStartUpVector;
	real m_vStartFOV;

	CShader* m_pSDEdited;

	QTimer m_SleepToolBarTimer;
};

#endif // FragEditor_H
