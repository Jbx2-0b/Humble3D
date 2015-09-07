#include "CParamShaderItem.h"
#include "CNodeTreeWidget.h"

//-----------------------------------------------------------------------------------------
CParamShaderItem::CParamShaderItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent /*= 0*/, bool bEditable /*= false*/)
    : CTreeWidgetItemBase(pTreeWidget, pParent)
{
    m_pWidget->setEnabled(bEditable);
}

//-----------------------------------------------------------------------------------------
CParamShaderItem::~CParamShaderItem()
{

}

//-----------------------------------------------------------------------------------------
void CParamShaderItem::setFilePathName(const QString& filePathName)
{
    m_pWidget->setFilePathName(filePathName);
}

//-----------------------------------------------------------------------------------------
QString CParamShaderItem::getFilePathName() const
{
    return m_pWidget->getFilePathName();
}
