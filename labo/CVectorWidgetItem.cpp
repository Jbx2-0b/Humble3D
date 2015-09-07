#include "CVectorWidgetItem.h"
#include "CNodeTreeWidget.h"

//-----------------------------------------------------------------------------------------
CVectorWidgetItem::CVectorWidgetItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent /*= 0*/, CVectorWidget::EnumType eWidgetType /*= eVectorWidget4D*/)
    : CTreeWidgetItemBase(pTreeWidget, pParent)
{
    m_pWidget->setType(eWidgetType);
    connect(m_pWidget, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
}

//-----------------------------------------------------------------------------------------
CVectorWidgetItem::~CVectorWidgetItem()
{

}

//-----------------------------------------------------------------------------------------
CVectorWidget* CVectorWidgetItem::vectorWidget()
{
    return m_pWidget;
}

//-----------------------------------------------------------------------------------------
void CVectorWidgetItem::onEditingFinished()
{
    m_pTreeWidget->onItemChanged(this, 0);
}
