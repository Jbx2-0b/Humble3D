#ifndef CTOOLBAR_H
#define CTOOLBAR_H

// Lib
#include "lib_ui_global.h"

// Foundations
#include "CGLGlobal.h"
#include "CGraphicsWidgetItem.h"
#include "CMath.h"

// Qt
#include <QFrame>
#include <QGraphicsView>
#include <QLayout>
#include <QPushButton>
#include <QLabel>


static const int iDefaultToolBarThickness = 50;
static const int iDefaultToolBarMargins = 2;

class LIB_UI_SHARED_EXPORT CToolBar : public QFrame
{
	Q_OBJECT

public:

	enum EnumToolBarPosition
	{
		eToolBarTop,
		eToolBarBottom,
		eToolBarRight,
		eToolBarLeft
	};

	enum EnumWidgetPosition
	{
		eWidgetRight,
		eWidgetLeft
	};

	enum EnumState
	{
		eOpened,
		eClosed,
		eOnOpen,
		eOnClose
	};

	CToolBar(EnumToolBarPosition ePosition, const QSize& viewSize, CGraphicsWidgetItem* pItem, QWidget* parent = 0);

	virtual ~CToolBar();

	template <class Widget>
	Widget* createWidget(EnumWidgetPosition eButtonPosition = eWidgetLeft, int iWidth = 0)
	{
		Widget* pWidget = new Widget(m_pLayoutFrame);
		m_Widgets.append(ToolBarWidget(pWidget, eButtonPosition));

		switch (m_ePosition)
		{
		case eToolBarTop:
		case eToolBarBottom:
			pWidget->setFixedWidth(iWidth == 0 ? m_iThickness - (2 * m_iMargins + 2) : iWidth);
			pWidget->setFixedHeight(m_iThickness - (2 * m_iMargins + 2));
			break;

		case eToolBarRight:
		case eToolBarLeft:
			pWidget->setFixedWidth(m_iThickness - (2 * m_iMargins + 2));
			pWidget->setFixedHeight(iWidth == 0 ? m_iThickness - (2 * m_iMargins + 2) : iWidth);
			break;
		}
		
		updateWidgetPositions();

		return pWidget;
	}

	void removeWidget(QWidget* pWidget);

	void setTitle(const QString& title);

	void setStyleSheetFile(const QString& styleSheetFileName);

	void setStyleSheet(const QString& styleSheet);

	void setOpacity(real dOpacity);

	void setThickness(int iThickness);

	void setMargins(int iMargins);

	void open();

	void close();

	EnumState getState() const { return m_eCurrentState; }

protected slots:

	void onViewSizeChanged(const QSize& size);
	void animateOpen();
	void animateClose();

protected:

	void updateWidgetPositions();

	CGraphicsWidgetItem* m_pItem;

	QFrame* m_pLayoutFrame;

	EnumToolBarPosition m_ePosition;

	QLayout* m_pLayout;

	QSpacerItem* m_pLeftSpacer;
	QSpacerItem* m_pRightSpacer;

	QSize m_ViewSize;

	QLabel m_TitleLabel;

	class ToolBarWidget
	{

	public:

		ToolBarWidget(QWidget* pWidget, EnumWidgetPosition eWidgetPosition)
			: m_pWidget(pWidget), m_ePosition(eWidgetPosition)
		{}

		QWidget* getQWidget() const { return m_pWidget; }

		EnumWidgetPosition getPosition() const { return m_ePosition; }

		inline bool operator == (const ToolBarWidget& tbWidget) const
		{
			return getQWidget() == tbWidget.getQWidget();
		}

	private:

		QWidget* m_pWidget;

		EnumWidgetPosition m_ePosition;
	};

	QList<ToolBarWidget> m_Widgets;

	int m_iThickness;

	int m_iMargins;

	EnumState m_eCurrentState;

	int m_iDelta;
	int m_iTotalDelta;
	
	//! Temps en millisecondes entre deux étapes d'animation
	static const int StepAnimationDT = 0;
};

#endif // CTOOLBAR_H
