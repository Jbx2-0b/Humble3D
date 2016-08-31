#ifndef CVECTORWIDGETITEM_H
#define CVECTORWIDGETITEM_H

// Foundations
#include "CTreeWidgetItemBase.h"
#include "CVectorWidget.h"

class CVectorWidgetItem : public CTreeWidgetItemBase<CVectorWidget>
{
    Q_OBJECT

public:

    //! Constructeur
    CVectorWidgetItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0, CVectorWidget::EnumType eWidgetType = CVectorWidget::eVectorWidget4D);

    //! Destructeur
    virtual ~CVectorWidgetItem();

    //! Accesseur sur le widget
    CVectorWidget* vectorWidget();

protected slots:

    void onEditingFinished();

};

#endif // CVECTORWIDGETITEM_H
