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


//! Temps en millisecondes entre deux �tapes d'animation
static const int StepAnimationDT = 0;

//! Diff�rents placements des menus
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

    //! Etat pouvant �tre prit par le menu
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

    //! Retourne l'�tat courant
    EnumState getCurrentState() const { return m_eCurrentAction; }

    //! D�finit l'opacit� du panneau
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

    //! Signal envoy� lorsque le panel est ouvert
    void opened();

    //! Signal envoy� lorsque le panel est ferm�
    void closed();

    //! Signal envoy� lorsque le panel va s'ouvrir
    void opening();

    //! Signal envoy� lorsque le panel va se fermer
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

    //! Ctor (cr�e son bouton d'ouverture), s'adapte � une sc�ne
    CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget � d�corer
                            QGraphicsScene* pScene,								// Scene sur laquelle sera plac� l'�l�ment
                            EnumPlacement ePlacement,							// Emplacement du menu
                            int iScreenOffset,									// Offset Horizontal ou Vertical du menu
                            const QString& openPanelIconFileName,				// Icone � afficher sur le bouton d'ouverture
                            const QString& openPanelText = "",					// Text � afficher sur le bouton d'ouverture
                            int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
                            const QString& openIcon = "",						// Icone d'ouverture
                            const QString& closeIcon = "");						// Icone de fermeture

    //! Ctor (cr�e son bouton d'ouverture), s'adapte � un item
    CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget � d�corer
                            QGraphicsItem* pItem,								// Item sur lequelle sera plac� l'�l�ment
                            EnumPlacement ePlacement,							// Emplacement du menu
                            int iScreenOffset,									// Offset Horizontal ou Vertical du menu
                            const QString& openPanelIconFileName,				// Icone � afficher sur le bouton d'ouverture
                            const QString& openPanelText = "",					// Text � afficher sur le bouton d'ouverture
                            int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
                            const QString& openIcon = "",						// Icone d'ouverture
                            const QString& closeIcon = "");						// Icone de fermeture


    //! Ctor (On fournit le bouton d'ouverture), s'adapte � une sc�ne
    CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget � d�corer
                            QGraphicsScene* pScene,								// Scene sur laquelle sera plac� l'�l�ment
                            EnumPlacement ePlacement,							// Emplacement du menu
                            int iScreenOffset,									// Offset Horizontal ou Vertical du menu
                            QWidget* pOpenPanelButton,							// Pointeur sur le bouton d'ouverture
                            int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
                            const QString& openIcon = "",						// Icone d'ouverture
                            const QString& closeIcon = "");						// Icone de fermeture

    //! Ctor (On fournit le bouton d'ouverture), s'adapte � un item
    CAnimatedPanelDecorator(QWidget* pDecoratedWidget,							// Widget � d�corer
                            QGraphicsItem* pItem,								// Scene sur laquelle sera plac� l'�l�ment
                            EnumPlacement ePlacement,							// Emplacement du menu
                            int iScreenOffset,									// Offset Horizontal ou Vertical du menu
                            QWidget* pOpenPanelButton,							// Pointeur sur le bouton d'ouverture
                            int iButtonOffset = 20,								// Offset Horizontal ou Vertical du bouton par rapport au menu
                            const QString& openIcon = "",						// Icone d'ouverture
                            const QString& closeIcon = "");						// Icone de fermeture



    //! Dtor
    virtual ~CAnimatedPanelDecorator();

    //! D�finit si le panneau est visible
    virtual void setVisible(bool bVisible);

    //! D�finit l'opacit� du panneau
    virtual void setOpacity(real dOpacity);

    //! D�finit la feuille de style appliqu�e sur le panneau
    void setStyleSheet(const QString& styleSheet);

    //! Retourne true si est ouvert
    bool isOpen() { return m_pPanelAnimator ? m_pPanelAnimator->getCurrentState() == PanelAnimator::eOpened : false; }

    //! Retourne true si est ferm�
    bool isClose() { return m_pPanelAnimator ? m_pPanelAnimator->getCurrentState() == PanelAnimator::eClosed : false; }

public slots:

    //! Ouvre le panneau
    void open();

    //! Ferme le panneau
    void close();

signals:

    //! Signal envoy� lorsque le panel est ouvert
    void opened();

    //! Signal envoy� lorsque le panel est ferm�
    void closed();

    //! Signal envoy� lorsque le panel va s'ouvrir
    void opening();

    //! Signal envoy� lorsque le panel va se fermer
    void closing();

private:

    PanelAnimator* m_pPanelAnimator;
    QPushButton* m_pOpenPanelButton;
    QWidget* m_pDecoratedWidget;
};


#endif // ANIMATEDPANELDECORATOR_H
