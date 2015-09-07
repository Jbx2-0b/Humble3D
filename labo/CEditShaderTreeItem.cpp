#include "CEditShaderTreeItem.h"
#include "CNodeTreeWidget.h"

//-----------------------------------------------------------------------------------------
CEditShaderTreeItem::CEditShaderTreeItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent /*= 0*/, bool bWidthEditButton /*= false*/)
    : CTreeWidgetItemBase(pTreeWidget, pParent)
{
    m_pWidget->setButtonVisible(bWidthEditButton);
    connect(m_pWidget, SIGNAL(editShader(const QString&)), m_pTreeWidget, SIGNAL(editShader(const QString&)));
}

//-----------------------------------------------------------------------------------------
CEditShaderTreeItem::~CEditShaderTreeItem()
{

}

//-----------------------------------------------------------------------------------------
void CEditShaderTreeItem::setName(const QString& name)
{
    m_pWidget->setText(name);
}

//-----------------------------------------------------------------------------------------
QString CEditShaderTreeItem::getName() const
{
    return m_pWidget->getText();
}
