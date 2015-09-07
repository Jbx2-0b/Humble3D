#ifndef ANIMATEDPANELDECORATOR_H
#define ANIMATEDPANELDECORATOR_H

// Lib
#include "lib_ui_global.h"

// Foundations
#include "CGraphicsWidgetItem.h"
#include "CMath.h"

// Qt
#include <QWidget>
#include <QObject>
#include <QPushButton>


//! Temps en millisecondes entre deux étapes d'animation
static const int StepAnimationDT = 0;

//! Différents placements des menus
enum EnumPlacement
{
	eTop,
	eBottom,
	eRight,
	eLeft
};

//-----------------------------------------------------------------------
// PANEL ANIMATOR
//-----------------------------------------------------------------------
//! @brief Permet d'animer un widget afin d'en faire un menu animable
//! @author Jean-Baptiste Tymen
class PanelAnimator : public QObject
{
	Q_OBJECT

public:

	//! Etat pouvant être prit par le menu
	enum EnumState
	{
		eOpened,
		eClosed,
		eOnOpen,
		eOnClose
	};

	//! Ctor
	PanelAnimator(QGraphicsScene* pScene, 
				  QWidget* pCentralWidget, 
				  EnumPlacement ePlacement = eLeft, 
				  int iScreenOffset = 50,
				  QWidget* pBorderWidget = 0,
				  bool bEmbeddedWidget = true,
				  int iButtonOffset = 20,
				  const QString& openIcon = "",						// Icone d'ouverture
				  const QString& closeIcon = "");						// Icone de fermeture

	//! Ctor
	PanelAnimator(QGraphicsItem* pItem,
				  QWidget* pCentralWidget, 
				  EnumPlacement ePlacement = eLeft, 
				  int iScreenOffset = 50, 
				  QWidget* pBorderWidget = 0,
				  bool bEmbeddedWidget = true,
				  int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
				  const QString& openIcon = "",						// Icone d'ouverture
				  const QString& closeIcon = "");						// Icone de fermeture

	//! Dtor
	virtual ~PanelAnimator();

	//! Anime l'ouverture
	void open();

	//! Anime la fermeture
	void close();

	//! Retourne l'état courant
	EnumState getCurrentState() const { return m_eCurrentAction; }

	//! Définit l'opacité du panneau
	void setOpacity(real dOpacity);

public slots:

	//! Anime l'ouverture / fermeture du menu
	void animate();

protected slots:

	//! Anime l'ouverture
	void animateOpen();

	//! Anime la fermeture
	void animateClose();

signals:

	//! Signal envoyé lorsque le panel est ouvert
	void opened();

	//! Signal envoyé lorsque le panel est fermé
	void closed();

	//! Signal envoyé lorsque le panel va s'ouvrir
	void opening();

	//! Signal envoyé lorsque le panel va se fermer
	void closing();

private:

	//! Mode d'attachement du menu
	enum EnumClipMode
	{
		eScene,
		eItem
	};

	CGraphicsWidgetItem* m_pProxyBorderWidget;
	CGraphicsWidgetItem* m_pProxyCentralWidget;
	
	int m_iDelta;
	int m_iTotalDelta;

	EnumState m_eCurrentAction;
	EnumPlacement m_ePlacement;
	EnumClipMode m_eMode;

	QGraphicsScene* m_pScene;
	QGraphicsItem* m_pItem;

	//! Icone d'ouverture
	QIcon m_OpenIcon;

	//! Icone de fermeture
	QIcon m_CloseIcon;

	//! Pointeur sur le widget d'ouverture
	QWidget* m_pBorderWidget;
};


//-----------------------------------------------------------------------
// PANEL DECORATOR
//-----------------------------------------------------------------------

//! @brief Permet de decorer un widget afin d'en faire un menu animable
//! @author Jean-Baptiste Tymen
//! @date 17/02/2012
class LIB_UI_SHARED_EXPORT CAnimatedPanelDecorator : public QObject
{
	Q_OBJECT

public:

	//! Ctor (crée son bouton d'ouverture), s'adapte à une scène
	CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget à décorer
						   QGraphicsScene* pScene,								// Scene sur laquelle sera placé l'élément
						   EnumPlacement ePlacement,							// Emplacement du menu
						   int iScreenOffset,									// Offset Horizontal ou Vertical du menu
						   const QString& openPanelIconFileName,				// Icone à afficher sur le bouton d'ouverture
						   const QString& openPanelText = "",					// Text à afficher sur le bouton d'ouverture
						   int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
						   const QString& openIcon = "",						// Icone d'ouverture
						   const QString& closeIcon = "");						// Icone de fermeture

	//! Ctor (crée son bouton d'ouverture), s'adapte à un item
	CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget à décorer
						   QGraphicsItem* pItem,								// Item sur lequelle sera placé l'élément
						   EnumPlacement ePlacement,							// Emplacement du menu
						   int iScreenOffset,									// Offset Horizontal ou Vertical du menu
						   const QString& openPanelIconFileName,				// Icone à afficher sur le bouton d'ouverture
						   const QString& openPanelText = "",					// Text à afficher sur le bouton d'ouverture
						   int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
						   const QString& openIcon = "",						// Icone d'ouverture
						   const QString& closeIcon = "");						// Icone de fermeture


	//! Ctor (On fournit le bouton d'ouverture), s'adapte à une scène
	CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget à décorer
						   QGraphicsScene* pScene,								// Scene sur laquelle sera placé l'élément
						   EnumPlacement ePlacement,							// Emplacement du menu
						   int iScreenOffset,									// Offset Horizontal ou Vertical du menu
						   QWidget* pOpenPanelButton,							// Pointeur sur le bouton d'ouverture
						   int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
						   const QString& openIcon = "",						// Icone d'ouverture
						   const QString& closeIcon = "");						// Icone de fermeture

	//! Ctor (On fournit le bouton d'ouverture), s'adapte à un item
	CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget à décorer
						   QGraphicsItem* pItem,								// Scene sur laquelle sera placé l'élément
						   EnumPlacement ePlacement,							// Emplacement du menu
						   int iScreenOffset,									// Offset Horizontal ou Vertical du menu
						   QWidget* pOpenPanelButton,							// Pointeur sur le bouton d'ouverture
						   int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
						   const QString& openIcon = "",						// Icone d'ouverture
						   const QString& closeIcon = "");						// Icone de fermeture



	//! Dtor
	virtual ~CAnimatedPanelDecorator();

	//! Définit si le panneau est visible
	virtual void setVisible(bool bVisible);

	//! Définit l'opacité du panneau
	virtual void setOpacity(real dOpacity);

	//! Définit la feuille de style appliquée sur le panneau
	void setStyleSheet(const QString& styleSheet);

	//! Retourne true si est ouvert
	bool isOpen() { return m_pPanelAnimator ? m_pPanelAnimator->getCurrentState() == PanelAnimator::eOpened : false; }

	//! Retourne true si est fermé
	bool isClose() { return m_pPanelAnimator ? m_pPanelAnimator->getCurrentState() == PanelAnimator::eClosed : false; }

public slots:

	//! Ouvre le panneau
	void open();

	//! Ferme le panneau
	void close();

signals:

	//! Signal envoyé lorsque le panel est ouvert
	void opened();

	//! Signal envoyé lorsque le panel est fermé
	void closed();

	//! Signal envoyé lorsque le panel va s'ouvrir
	void opening();

	//! Signal envoyé lorsque le panel va se fermer
	void closing();

private:

    PanelAnimator* m_pPanelAnimator;
    QPushButton* m_pOpenPanelButton;
	QWidget* m_pDecoratedWidget;
};


#endif // ANIMATEDPANELDECORATOR_H
