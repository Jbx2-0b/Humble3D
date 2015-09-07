#ifndef TERRAINVIEWERAPP_H
#define TERRAINVIEWERAPP_H

#include "CLight.h"
#include "CGraphicsView3D.h"
#include "CExclusiveButton.h"
#include "CTerrainManager.h"

class CToolBar;


static const int iDefaultViewSizeX = 1200;
static const int iDefaultViewSizeY = 675;
static const int iDefaultViewPosX = 100;
static const int iDefaultViewPosY = 100;

class TerrainViewerApp : public CGraphicsView3D
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
	TerrainViewerApp(QWidget* parent = 0);

	//! Destructeur
	virtual ~TerrainViewerApp();

protected slots:

	void onMousePressed();
	void onMouseMoved();
	void onMouseReleased();
	void onKeyPressed();
	void onViewSizeChanged(const QSize& size);
	void onFullScreen();
	void onRunEditSwitch();
	void onOptionChanged();
	void onFPSChanged(int iFPS);

protected:

	static const int iBorderEditItem = 2;
	static const int iTreeWidgetWidth = 300;

	void wheel(int delta);
	void pan(int deltax, int deltay);
	void rotate(int deltax, int deltay);
	void fov(int delta);

	//! Barre d'outils supérieure
	CToolBar* m_pTopToolBar;

	//! Barre d'outils inférieure
	CToolBar* m_pBottomToolBar;

	//! Bouton plein écran
	QPushButton* m_pRunButton;
	QPushButton* m_pFullScreenButton;
	QPushButton* m_pZoomButton;
	QPushButton* m_pRotateButton;
	QPushButton* m_pPanButton;
	QPushButton* m_pFOVButton;
	QPushButton* m_pResetButton;
	QPushButton* m_pAxeXZButton;
	QPushButton* m_pAxeYButton;

	QLabel* m_pFPSLabel;

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

	CTerrainManager* m_pTerrainManager;
};

#endif // TERRAINVIEWERAPP_H
