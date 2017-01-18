#include "CToolBar.h"

// Qt
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

//-----------------------------------------------------------------------
CToolBar::CToolBar(CToolBar::EnumToolBarPosition ePosition, const QSize& viewSize, QSharedPointer<CGraphicsWidgetItem> pItem, QWidget* parent /*= 0*/)
    : QFrame(parent)
    , m_pItem(pItem)
    , m_ePosition(ePosition)
    , m_ViewSize(viewSize)
    , m_iThickness(iDefaultToolBarThickness)
    , m_iMargins(iDefaultToolBarMargins)
    , m_eCurrentState(eOpened)
    , m_iDelta(1)
    , m_iTotalDelta(0)
{
    m_pLayoutFrame = new QFrame(this);

    switch (ePosition)
    {
    case eToolBarTop:
    case eToolBarBottom:
        resize(m_ViewSize.width(), m_iThickness);
        m_pLayout = new QHBoxLayout(m_pLayoutFrame);
        break;

    case eToolBarRight:
    case eToolBarLeft:
        resize(m_iThickness, m_ViewSize.height());
        m_pLayout = new QVBoxLayout(m_pLayoutFrame);
        break;
    }

    m_pLayoutFrame->setGeometry(0, 0, width(), height());

    m_pLayout->setSpacing(m_iMargins);
    m_pLayout->setContentsMargins(m_iMargins, m_iMargins, m_iMargins, m_iMargins);

    m_pLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_pRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // On place le panneau en postion initiale en fonction de son emplacement sur l'écran
    switch (ePosition)
    {
    case eToolBarLeft:
        move(-pItem->width() + 1, 0);
        break;
    case eToolBarRight:
        move(pItem->boundingRect().width(), 0);
        break;
    case eToolBarTop:
        move(0, -pItem->height());
        break;
    case eToolBarBottom:
        move(0, pItem->boundingRect().height());
        break;
    }
}

//-----------------------------------------------------------------------
CToolBar::~CToolBar()
{
    m_pLayout->removeItem(m_pLeftSpacer);
    delete m_pLeftSpacer;

    m_pLayout->removeItem(m_pRightSpacer);
    delete m_pRightSpacer;

    for (const ToolBarWidget& widget : m_Widgets)
    {
        m_pLayout->removeWidget(widget.getQWidget());
        m_Widgets.removeAll(widget);
        delete widget.getQWidget();
    }
}

//-----------------------------------------------------------------------
void CToolBar::updateWidgetPositions()
{
    m_pLayout->removeItem(m_pLeftSpacer);
    m_pLayout->removeItem(m_pRightSpacer);

    m_pLayout->removeWidget(&m_TitleLabel);

    for (const ToolBarWidget& widget : m_Widgets)
    {
        m_pLayout->removeWidget(widget.getQWidget());
    }

    for (const ToolBarWidget& widget : m_Widgets)
    {
        if (widget.getPosition() == eWidgetLeft)
        {
            m_pLayout->addWidget(widget.getQWidget());
        }
    }

    m_pLayout->addItem(m_pLeftSpacer);
    m_pLayout->addWidget(&m_TitleLabel);
    m_pLayout->addItem(m_pRightSpacer);


    for (const ToolBarWidget& widget : m_Widgets)
    {
        if (widget.getPosition() == eWidgetRight)
        {
            m_pLayout->addWidget(widget.getQWidget());
        }
    }
}
//-----------------------------------------------------------------------
void CToolBar::removeWidget(QWidget* pWidget)
{
    m_pLayout->removeWidget(pWidget);
    delete pWidget;
}

//-----------------------------------------------------------------------
void CToolBar::setTitle(const QString& title)
{
    m_TitleLabel.setText(title);
    updateWidgetPositions();
}

//-----------------------------------------------------------------------
void CToolBar::setStyleSheetFile(const QString& styleSheetFileName)
{
    QFile styleFile(styleSheetFileName);

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_pLayoutFrame->setStyleSheet(QString::fromUtf8(styleFile.readAll()));
    }

    styleFile.close();
}

//-----------------------------------------------------------------------
void CToolBar::setStyleSheet(const QString& styleSheet)
{
    m_pLayoutFrame->setStyleSheet(styleSheet);
}

//-----------------------------------------------------------------------
void CToolBar::setOpacity(real dOpacity)
{
    m_pItem->setOpacity(dOpacity);
}

//-----------------------------------------------------------------------
void CToolBar::setThickness(int iThickness)
{
    m_iThickness = iThickness;

    onViewSizeChanged(m_ViewSize);
}

//-----------------------------------------------------------------------
void CToolBar::setMargins(int iMargins)
{
    m_iMargins = iMargins;
    m_pLayoutFrame->setContentsMargins(iMargins, iMargins, iMargins, iMargins);
}

//-----------------------------------------------------------------------
void CToolBar::onViewSizeChanged(const QSize& viewSize)
{
    m_ViewSize = viewSize;

    switch (m_ePosition)
    {
    case eToolBarTop:
        resize(m_ViewSize.width(), m_iThickness);
        break;

    case eToolBarBottom:
        move(0, m_ViewSize.height() - m_iThickness);
        resize(m_ViewSize.width(), m_iThickness);
        break;


    case eToolBarRight:
    case eToolBarLeft:
        resize(m_iThickness, m_ViewSize.height());
        break;
    }

    m_pLayoutFrame->setGeometry(0, 0, width(), height());
}

//-----------------------------------------------------------------------
void CToolBar::open()
{
#ifdef DESKTOP_TARGET
    m_iTotalDelta = 0;
    animateOpen();
#endif
}

//-----------------------------------------------------------------------
void CToolBar::close()
{
#ifdef DESKTOP_TARGET
    m_iTotalDelta = 0;
    animateClose();
#endif
}

//-----------------------------------------------------------------------
void CToolBar::animateOpen()
{
    int iStopCoord = 0;
    QRectF r = m_pItem->boundingRect();

    if (m_eCurrentState == eClosed || m_eCurrentState == eOnOpen)
    {
        m_eCurrentState = eOnOpen;

        m_iTotalDelta += m_iDelta;

        // On deplace le panel en fonction de son placement
        switch (m_ePosition)
        {
        case eToolBarRight:
            m_pItem->setTransform(QTransform().translate(+ r.width() - m_iTotalDelta, 0));
            iStopCoord = r.width();
            break;
        case eToolBarLeft:
            m_pItem->setTransform(QTransform().translate(- r.width() + m_iTotalDelta, 0));
            iStopCoord = r.width();
            break;
        case eToolBarTop:
            m_pItem->setTransform(QTransform().translate(0, - r.height() + m_iTotalDelta));
            iStopCoord = r.height();
            break;
        case eToolBarBottom:
            m_pItem->setTransform(QTransform().translate(0, + r.height() -m_iTotalDelta));
            iStopCoord = r.height();
            break;
        }
    }
    if (m_iTotalDelta < iStopCoord)
    {
        QTimer::singleShot(StepAnimationDT, this, SLOT(animateOpen()));
    }
    else
    {
        m_eCurrentState = eOpened;

        m_pItem->setTransform(QTransform());
    }
}

//-----------------------------------------------------------------------
void CToolBar::animateClose()
{
    QRectF r = m_pItem->boundingRect();

    int iStopCoord = 0;

    if (m_eCurrentState == eOpened || m_eCurrentState == eOnClose)
    {
        m_eCurrentState = eOnClose;

        m_iTotalDelta += m_iDelta;

        // On deplace le panel en fonction de son placement
        switch (m_ePosition)
        {
        case eToolBarRight:
            m_pItem->setTransform(QTransform().translate(+ m_iTotalDelta, 0));
            iStopCoord = r.width();
            break;
        case eToolBarLeft:
            m_pItem->setTransform(QTransform().translate(- m_iTotalDelta, 0));
            iStopCoord = r.width();
            break;
        case eToolBarTop:
            m_pItem->setTransform(QTransform().translate(0, - m_iTotalDelta));
            iStopCoord = r.height();
            break;
        case eToolBarBottom:
            m_pItem->setTransform(QTransform().translate(0, + m_iTotalDelta));
            iStopCoord = r.height();
            break;
        }
    }

    if (m_iTotalDelta < iStopCoord)
    {
        QTimer::singleShot(StepAnimationDT, this, SLOT(animateClose()));
    }
    else
    {
        m_eCurrentState = eClosed;

        switch (m_ePosition)
        {
        case eToolBarRight:
            m_pItem->setTransform(QTransform().translate(+ r.width(), 0));
            break;
        case eToolBarLeft:
            m_pItem->setTransform(QTransform().translate(- r.width(), 0));
            break;
        case eToolBarTop:
            m_pItem->setTransform(QTransform().translate(0, - r.height()));
            break;
        case eToolBarBottom:
            m_pItem->setTransform(QTransform().translate(0, + r.height()));
            break;
        }
    }
}
