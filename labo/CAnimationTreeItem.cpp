#include "CAnimationTreeItem.h"
#include "CNodeTreeWidget.h"

//-----------------------------------------------------------------------------------------
CAnimationTreeItem::CAnimationTreeItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent /*= 0*/)
    : CTreeWidgetItemBase(pTreeWidget, pParent)
{
    connect(m_pWidget, SIGNAL(playAnimation(const QString&, bool)), m_pTreeWidget, SIGNAL(playAnimation(const QString&, bool)));
}

//-----------------------------------------------------------------------------------------
CAnimationTreeItem::~CAnimationTreeItem()
{

}

//-----------------------------------------------------------------------------------------
void CAnimationTreeItem::setName(const QString& name)
{
    m_pWidget->setText(name);
}

//-----------------------------------------------------------------------------------------
QString CAnimationTreeItem::getName() const
{
    return m_pWidget->getText();
}
