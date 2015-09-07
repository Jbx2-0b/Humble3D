#include "CAnimatedPanelDecorator.h"

#include <QTimer>
#include <QGraphicsScene>
#include <QPainter>

//-----------------------------------------------------------------------
// PANEL ANIMATOR
//-----------------------------------------------------------------------
PanelAnimator::PanelAnimator(QGraphicsScene* pScene, 
							 QWidget* pCentralWidget, 
							 EnumPlacement ePlacement, 
							 int iScreenOffset,
							 QWidget* pBorderWidget,
							 bool bEmbeddedWidget,
							 int iButtonOffset,
							 const QString& openIconFileName /*= ""*/,
							 const QString& closeIconFileName /*= ""*/)
: QObject(pScene)
, m_pProxyBorderWidget(0)
, m_pProxyCentralWidget(0)
, m_iDelta(10)
, m_iTotalDelta(0)
, m_eCurrentAction(eClosed)
, m_ePlacement(ePlacement)
, m_eMode(eScene)
, m_pScene(pScene)
, m_pItem(0)
, m_pBorderWidget(pBorderWidget)
{
	if (!openIconFileName.isEmpty()) m_OpenIcon = QIcon(openIconFileName);
	if (!closeIconFileName.isEmpty()) m_CloseIcon = QIcon(closeIconFileName);

	// On place le panneau en postion initiale en fonction de son emplacement sur l'écran
	switch (ePlacement)
	{
	case eLeft:
		pCentralWidget->move(-pCentralWidget->width() + 1, iScreenOffset);
		if (pBorderWidget) pBorderWidget->move(- 1, iScreenOffset + iButtonOffset);
		break;
	case eRight:
		pCentralWidget->move(pScene->width(), iScreenOffset);
		if (pBorderWidget) pBorderWidget->move(pScene->width() - pBorderWidget->width() + 1, iScreenOffset  + iButtonOffset);
		break;
	case eTop:
		pCentralWidget->move(iScreenOffset, -pCentralWidget->height());
		if (pBorderWidget) pBorderWidget->move(iScreenOffset  + iButtonOffset, 0);
		break;
	case eBottom:
		pCentralWidget->move(iScreenOffset, pScene->height());
		if (pBorderWidget) pBorderWidget->move(iScreenOffset  + iButtonOffset, pScene->height() - pBorderWidget->height());
		break;
	}

	m_pProxyCentralWidget = new CGraphicsWidgetItem;
	pCentralWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_pProxyCentralWidget->setWidget(pCentralWidget);
	m_pProxyCentralWidget->setCacheMode(QGraphicsItem::ItemCoordinateCache);
	m_pProxyCentralWidget->setZValue(1e30);
	m_pScene->addItem(m_pProxyCentralWidget);

	if (pBorderWidget) 
	{
		if (bEmbeddedWidget)
		{
			m_pProxyBorderWidget = new CGraphicsWidgetItem;
			pBorderWidget->setAttribute(Qt::WA_TranslucentBackground);
			m_pProxyBorderWidget->setWidget(pBorderWidget);
			m_pProxyBorderWidget->setCacheMode(QGraphicsItem::ItemCoordinateCache);
			m_pProxyBorderWidget->setZValue(1e30);
			m_pScene->addItem(m_pProxyBorderWidget);
		}

		connect(pBorderWidget, SIGNAL(clicked()), this, SLOT(animate()));

		if (!m_OpenIcon.isNull())
		{
			if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
			{
				pBtn->setIcon(m_OpenIcon);
			}
		}
	}
}

//-----------------------------------------------------------------------
// PANEL ANIMATOR
//-----------------------------------------------------------------------
PanelAnimator::PanelAnimator(QGraphicsItem* pItem, 
							 QWidget* pCentralWidget, 
							 EnumPlacement ePlacement, 
							 int iScreenOffset,
							 QWidget* pBorderWidget,
							 bool bEmbeddedWidget,
							 int iButtonOffset,
							 const QString& openIconFileName /*= ""*/,
							 const QString& closeIconFileName /*= ""*/)
: m_pProxyBorderWidget(0)
, m_pProxyCentralWidget(0)
, m_iDelta(10)
, m_iTotalDelta(0)
, m_eCurrentAction(eClosed)
, m_ePlacement(ePlacement)
, m_eMode(eItem)
, m_pScene(0)
, m_pItem(pItem)
, m_pBorderWidget(pBorderWidget)
{
	if (!openIconFileName.isEmpty()) m_OpenIcon = QIcon(openIconFileName);
	if (!closeIconFileName.isEmpty()) m_CloseIcon = QIcon(closeIconFileName);

	// On place le panneau en postion initiale en fonction de son emplacement sur l'écran
	switch (ePlacement)
	{
	case eLeft:
		pCentralWidget->move(-pCentralWidget->width() + 1, iScreenOffset);
		if (pBorderWidget) pBorderWidget->move(- 1, iScreenOffset + iButtonOffset);
		break;
	case eRight:
		pCentralWidget->move(pItem->boundingRect().width(), iScreenOffset);
		if (pBorderWidget) pBorderWidget->move(pItem->boundingRect().width() - pBorderWidget->width() + 1, iScreenOffset  + iButtonOffset);
		break;
	case eTop:
		pCentralWidget->move(iScreenOffset, -pCentralWidget->height());
		if (pBorderWidget) pBorderWidget->move(iScreenOffset  + iButtonOffset, 0);
		break;
	case eBottom:
		pCentralWidget->move(iScreenOffset, pItem->boundingRect().height());
		if (pBorderWidget) pBorderWidget->move(iScreenOffset  + iButtonOffset, pItem->boundingRect().height() - pBorderWidget->height());
		break;
	}

	m_pProxyCentralWidget = new CGraphicsWidgetItem(pItem);
	pCentralWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_pProxyCentralWidget->setWidget(pCentralWidget);
	m_pProxyCentralWidget->setCacheMode(QGraphicsItem::ItemCoordinateCache);
	m_pProxyCentralWidget->setZValue(1e30);
	

	if (pBorderWidget) 
	{
		if (bEmbeddedWidget)
		{
			m_pProxyBorderWidget = new CGraphicsWidgetItem(pItem);
			pBorderWidget->setAttribute(Qt::WA_TranslucentBackground);
			m_pProxyBorderWidget->setWidget(pBorderWidget);
			m_pProxyBorderWidget->setCacheMode(QGraphicsItem::ItemCoordinateCache);
			m_pProxyBorderWidget->setZValue(1e30);
		}

		connect(pBorderWidget, SIGNAL(clicked()), this, SLOT(animate()));

		if (!m_OpenIcon.isNull())
		{
			if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
			{
				pBtn->setIcon(m_OpenIcon);
			}
		}
	}
}

//-----------------------------------------------------------------------
PanelAnimator::~PanelAnimator()
{
	disconnect();
	m_pProxyCentralWidget->setWidget(0);


	if (m_pItem)
	{
		m_pItem->setParentItem(0);
	}

	if (m_pProxyBorderWidget)
	{
		m_pProxyBorderWidget->setWidget(0);

		// Selon le mode, on détache l'pItem de la scéne ou de son pItem parent
		if (m_eMode == eScene)
		{
			m_pScene->removeItem(m_pProxyBorderWidget);
		}
		else // eItem
		{
			m_pProxyBorderWidget->setParentItem(0);
		}

		delete m_pProxyBorderWidget;
	}
	
	// Selon le mode, on détache l'pItem de la scéne ou de son pItem parent
	if (m_eMode == eScene)
	{
		m_pScene->removeItem(m_pProxyCentralWidget);
	}
	else // eItem
	{
		m_pProxyCentralWidget->setParentItem(0);
	}

	delete m_pProxyCentralWidget;
}

//-----------------------------------------------------------------------
void PanelAnimator::animate()
{
	if (m_eCurrentAction == eClosed)
	{
		if (!m_CloseIcon.isNull())
		{
			if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
			{
				pBtn->setIcon(m_CloseIcon);
			}
		}
		
		m_iTotalDelta = 0;
		emit opening();
		animateOpen();
	}
	else if (m_eCurrentAction == eOpened)
	{
		if (!m_OpenIcon.isNull())
		{
			if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
			{
				pBtn->setIcon(m_OpenIcon);
			}
		}
		
		m_iTotalDelta = 0;
		emit closing();
		animateClose();
	}
}

//-----------------------------------------------------------------------
void PanelAnimator::open()
{
	if (!m_CloseIcon.isNull())
	{
		if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
		{
			pBtn->setIcon(m_CloseIcon);
		}
	}

	m_iTotalDelta = 0;
	emit opening();
	animateOpen();
}

//-----------------------------------------------------------------------
void PanelAnimator::close()
{
	if (!m_OpenIcon.isNull())
	{
		if (QPushButton* pBtn = dynamic_cast<QPushButton*>(m_pBorderWidget))
		{
			pBtn->setIcon(m_OpenIcon);
		}
	}

	m_iTotalDelta = 0;
	emit closing();
	animateClose();
}

//-----------------------------------------------------------------------
void PanelAnimator::setOpacity(real dOpacity)
{
	if (m_pProxyCentralWidget)
	{
		m_pProxyCentralWidget->setOpacity(dOpacity);
	}
}

//-----------------------------------------------------------------------
void PanelAnimator::animateOpen()
{
	m_eCurrentAction = eOnOpen;

	QRectF r = m_pProxyCentralWidget->boundingRect();

	m_iTotalDelta += m_iDelta;

    int iStopCoord = 0;

	// On deplace le panel en fonction de son placement
	switch (m_ePlacement)
	{
	case eRight:
		m_pProxyCentralWidget->setTransform(QTransform().translate(-m_iTotalDelta, 0));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(-m_iTotalDelta, 0));
		iStopCoord = r.width();
		break;
	case eLeft:
		m_pProxyCentralWidget->setTransform(QTransform().translate(+m_iTotalDelta, 0));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(+m_iTotalDelta, 0));
		iStopCoord = r.width();
		break;
	case eTop:
		m_pProxyCentralWidget->setTransform(QTransform().translate(0, +m_iTotalDelta));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, +m_iTotalDelta));
		iStopCoord = r.height();
		break;
	case eBottom:
		m_pProxyCentralWidget->setTransform(QTransform().translate(0, -m_iTotalDelta));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, -m_iTotalDelta));
		iStopCoord = r.height();
		break;
	}

	if (m_iTotalDelta < iStopCoord)
	{
		QTimer::singleShot(StepAnimationDT, this, SLOT(animateOpen()));
	}
	else
	{
		m_eCurrentAction = eOpened;

		// On s'assure d'être en butée
		switch (m_ePlacement)
		{
		case eRight:
			m_pProxyCentralWidget->setTransform(QTransform().translate(-r.width(), 0));
			if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(-r.width(), 0));
			break;
		case eLeft:
			m_pProxyCentralWidget->setTransform(QTransform().translate(+r.width(), 0));
			if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(+r.width(), 0));
			break;
		case eTop:
			m_pProxyCentralWidget->setTransform(QTransform().translate(0, +r.height()));
			if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, +r.height()));
			break;
		case eBottom:
			m_pProxyCentralWidget->setTransform(QTransform().translate(0, -r.height()));
			if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, -r.height()));
			break;
		}

		emit opened();
	}
}

//-----------------------------------------------------------------------
void PanelAnimator::animateClose()
{
	m_eCurrentAction = eOnClose;

	QRectF r = m_pProxyCentralWidget->boundingRect();

	m_iTotalDelta += m_iDelta;

    int iStopCoord = 0;

	// On deplace le panel en fonction de son placement
	switch (m_ePlacement)
	{
	case eRight:
		m_pProxyCentralWidget->setTransform(QTransform().translate(- r.width() + m_iTotalDelta, 0));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(- r.width() + m_iTotalDelta, 0));
		iStopCoord = r.width();
		break;
	case eLeft:
		m_pProxyCentralWidget->setTransform(QTransform().translate(r.width() - m_iTotalDelta, 0));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(r.width() - m_iTotalDelta, 0));
		iStopCoord = r.width();
		break;
	case eTop:
		m_pProxyCentralWidget->setTransform(QTransform().translate(0, r.height() - m_iTotalDelta));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, r.height() - m_iTotalDelta));
		iStopCoord = r.height();
		break;
	case eBottom:
		m_pProxyCentralWidget->setTransform(QTransform().translate(0, - r.height() + m_iTotalDelta));
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform().translate(0, - r.height() + m_iTotalDelta));
		iStopCoord = r.height();
		break;
	}

	if (m_iTotalDelta < iStopCoord)
	{
		QTimer::singleShot(StepAnimationDT, this, SLOT(animateClose()));
	}
	else
	{
		m_eCurrentAction = eClosed;
		// On s'assure d'être en butée
		m_pProxyCentralWidget->setTransform(QTransform());
		if (m_pProxyBorderWidget) m_pProxyBorderWidget->setTransform(QTransform());

		emit closed();
	}
}

//-----------------------------------------------------------------------
// PANEL DECORATOR
//-----------------------------------------------------------------------
CAnimatedPanelDecorator::CAnimatedPanelDecorator(QWidget* pDecoratedWidget,
											   QGraphicsScene* pScene,
											   EnumPlacement ePlacement,
											   int iScreenOffset,
											   const QString& openPanelIconFileName,
											   const QString& openPanelText,
											   int iButtonOffset,
											   const QString& openIcon /*= ""*/,
											   const QString& closeIcon /*= ""*/)
: m_pOpenPanelButton(0)
, m_pDecoratedWidget(pDecoratedWidget)
{
    m_pOpenPanelButton = new QPushButton();
    if (!openPanelIconFileName.isEmpty())
        m_pOpenPanelButton->setIcon(QIcon(openPanelIconFileName));
    m_pOpenPanelButton->setText(openPanelText);

	m_pPanelAnimator = new PanelAnimator(pScene, pDecoratedWidget, ePlacement, iScreenOffset, m_pOpenPanelButton, true, iButtonOffset, openIcon, closeIcon);

	connect(m_pPanelAnimator, SIGNAL(opened()),		this, SIGNAL(opened()));
	connect(m_pPanelAnimator, SIGNAL(closed()),		this, SIGNAL(closed()));
	connect(m_pPanelAnimator, SIGNAL(opening()),	this, SIGNAL(opening()));
	connect(m_pPanelAnimator, SIGNAL(closing()),	this, SIGNAL(closing()));
}

//-----------------------------------------------------------------------
CAnimatedPanelDecorator::CAnimatedPanelDecorator(QWidget* pDecoratedWidget,
											   QGraphicsItem* pItem,
											   EnumPlacement ePlacement,
											   int iScreenOffset,
											   const QString& openPanelIconFileName,
											   const QString& openPanelText,
											   int iButtonOffset,
											   const QString& openIcon /*= ""*/,
											   const QString& closeIcon /*= ""*/)
: m_pOpenPanelButton(0)
, m_pDecoratedWidget(pDecoratedWidget)
{
    m_pOpenPanelButton = new QPushButton();
    if (!openPanelIconFileName.isEmpty())
        m_pOpenPanelButton->setIcon(QIcon(openPanelIconFileName));
    m_pOpenPanelButton->setText(openPanelText);

	m_pPanelAnimator = new PanelAnimator(pItem, pDecoratedWidget, ePlacement, iScreenOffset, m_pOpenPanelButton, true, iButtonOffset, openIcon, closeIcon);

	pItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
		

	connect(m_pPanelAnimator, SIGNAL(opened()),		this, SIGNAL(opened()));
	connect(m_pPanelAnimator, SIGNAL(closed()),		this, SIGNAL(closed()));
	connect(m_pPanelAnimator, SIGNAL(opening()),	this, SIGNAL(opening()));
	connect(m_pPanelAnimator, SIGNAL(closing()),	this, SIGNAL(closing()));
}

//-----------------------------------------------------------------------
CAnimatedPanelDecorator::CAnimatedPanelDecorator(QWidget* pDecoratedWidget,
											   QGraphicsScene* pScene,
											   EnumPlacement ePlacement,
											   int iScreenOffset,
											   QWidget* pOpenPanelButton,
											   int iButtonOffset,
											   const QString& openIcon /*= ""*/,
											   const QString& closeIcon /*= ""*/)
: m_pOpenPanelButton(0)
, m_pDecoratedWidget(pDecoratedWidget)
{
	m_pPanelAnimator = new PanelAnimator(pScene, pDecoratedWidget, ePlacement, iScreenOffset, pOpenPanelButton, false, iButtonOffset, openIcon, closeIcon);

	if (pOpenPanelButton)
	{
		connect(pOpenPanelButton, SIGNAL(clicked()), m_pPanelAnimator, SLOT(animate()));
	}

	connect(m_pPanelAnimator, SIGNAL(opened()),		this, SIGNAL(opened()));
	connect(m_pPanelAnimator, SIGNAL(closed()),		this, SIGNAL(closed()));
	connect(m_pPanelAnimator, SIGNAL(opening()),	this, SIGNAL(opening()));
	connect(m_pPanelAnimator, SIGNAL(closing()),	this, SIGNAL(closing()));
}

//-----------------------------------------------------------------------
CAnimatedPanelDecorator::CAnimatedPanelDecorator(QWidget* pDecoratedWidget,
											   QGraphicsItem* pItem,
											   EnumPlacement ePlacement,
											   int iScreenOffset,
											   QWidget* pOpenPanelButton,
											   int iButtonOffset,
											   const QString& openIcon /*= ""*/,
											   const QString& closeIcon /*= ""*/)
: m_pOpenPanelButton(0)
, m_pDecoratedWidget(pDecoratedWidget)
{
	m_pPanelAnimator = new PanelAnimator(pItem, pDecoratedWidget, ePlacement, iScreenOffset, pOpenPanelButton, false, iButtonOffset, openIcon, closeIcon);

	if (pOpenPanelButton)
	{
		connect(pOpenPanelButton, SIGNAL(clicked()), m_pPanelAnimator, SLOT(animate()));
	}

	pItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);

	connect(m_pPanelAnimator, SIGNAL(opened()),		this, SIGNAL(opened()));
	connect(m_pPanelAnimator, SIGNAL(closed()),		this, SIGNAL(closed()));
	connect(m_pPanelAnimator, SIGNAL(opening()),	this, SIGNAL(opening()));
	connect(m_pPanelAnimator, SIGNAL(closing()),	this, SIGNAL(closing()));
}

//-----------------------------------------------------------------------
CAnimatedPanelDecorator::~CAnimatedPanelDecorator()
{
	delete m_pPanelAnimator;
	delete m_pOpenPanelButton;
}


//-----------------------------------------------------------------------
void CAnimatedPanelDecorator::setVisible(bool bVisible)
{
	if (m_pOpenPanelButton)
	{
		m_pOpenPanelButton->setVisible(bVisible);
	}

	if (m_pDecoratedWidget)
	{
		m_pDecoratedWidget->setVisible(bVisible);
	}
}

//-----------------------------------------------------------------------
void CAnimatedPanelDecorator::setOpacity(real dOpacity)
{
	if (m_pPanelAnimator)
	{
		m_pPanelAnimator->setOpacity(dOpacity);
	}
}

//-----------------------------------------------------------------------
void CAnimatedPanelDecorator::setStyleSheet(const QString& styleSheet)
{
	if (m_pDecoratedWidget)
	{
		m_pDecoratedWidget->setStyleSheet(styleSheet);
	}
	if (m_pOpenPanelButton)
	{
		m_pOpenPanelButton->setStyleSheet(styleSheet);
	}
}

//-----------------------------------------------------------------------
void CAnimatedPanelDecorator::open()
{
	if (m_pPanelAnimator)
	{
		if (m_pPanelAnimator->getCurrentState() == PanelAnimator::eClosed)
		{
			m_pPanelAnimator->open();
		}
	}
}

//-----------------------------------------------------------------------
void CAnimatedPanelDecorator::close()
{
	if (m_pPanelAnimator)
	{
		if (m_pPanelAnimator->getCurrentState() == PanelAnimator::eOpened)
		{
			m_pPanelAnimator->close();
		}
	}
}
