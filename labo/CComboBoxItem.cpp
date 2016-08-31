#include "CComboBoxItem.h"
#include "CNodeTreeWidget.h"

//-----------------------------------------------------------------------------------------
CComboBoxItem::CComboBoxItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent /*= 0*/)
    : CTreeWidgetItemBase(pTreeWidget, pParent)
{
    connect(m_pWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
}

//-----------------------------------------------------------------------------------------
CComboBoxItem::~CComboBoxItem()
{
}

//-----------------------------------------------------------------------------------------
void CComboBoxItem::addEntries(const QList<QString>& entries)
{
    m_pWidget->addItems(entries);
}

//-----------------------------------------------------------------------------------------
void CComboBoxItem::addEntry(const QString& entry)
{
    m_pWidget->addItem(entry);
}

//-----------------------------------------------------------------------------------------
void CComboBoxItem::removeEntry(const QString& entry)
{
    int idx = m_pWidget->findText(entry);
    if (idx >= 0)
    {
        m_pWidget->removeItem(idx);
    }
}

//-----------------------------------------------------------------------------------------
QString CComboBoxItem::getCurrentEntry() const
{
    return m_pWidget->currentText();
}

//-----------------------------------------------------------------------------------------
void CComboBoxItem::setCurrentEntry(const QString& entry)
{
    int idx = m_pWidget->findText(entry);
    if (idx >= 0)
    {
        m_pWidget->setCurrentIndex(idx);
    }
}

//-----------------------------------------------------------------------------------------
void CComboBoxItem::onCurrentIndexChanged(int)
{
    m_pTreeWidget->onItemChanged(this, 0);
}
