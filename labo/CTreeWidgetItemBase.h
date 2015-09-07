#ifndef TREEWIDGETITEMBASE_H
#define TREEWIDGETITEMBASE_H

// Qt
#include <QObject>
#include <QTreeWidgetItem>

#include "CNodeTreeWidget.h"

template<class Widget>
class CTreeWidgetItemBase : public QObject, public QTreeWidgetItem
{
public:

	//! Constructeur
    CTreeWidgetItemBase(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0)
        : QTreeWidgetItem()
        , m_pTreeWidget(pTreeWidget)
    {
        m_pWidget = new Widget(pTreeWidget);
        m_pWidget->setAutoFillBackground(true);

        if (pParent) pParent->addChild(this);
        m_pTreeWidget->setItemWidget(this, 0, m_pWidget);
    }

	//! Destructeur
	virtual ~CTreeWidgetItemBase()
	{
		// On ne supprime pas m_pWidget, le TreeWidget s'en charge
	}

	//! Active ou desactive le widget
	void setEnabled(bool bEnabled)
	{
		m_pWidget->setEnabled(bEnabled);
	}

protected:

	Widget* m_pWidget;
	CNodeTreeWidget* m_pTreeWidget;
};

#endif // TREEWIDGETITEMBASE_H
