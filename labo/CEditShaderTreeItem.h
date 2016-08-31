#ifndef CEDITSHADERTREEITEM_H
#define CEDITSHADERTREEITEM_H

// Foundations
#include "CTreeWidgetItemBase.h"
#include "CEditShaderTreeWidget.h"

class CEditShaderTreeItem : public CTreeWidgetItemBase<CEditShaderTreeWidget>
{
    Q_OBJECT

public:

    //! Constructeur
    CEditShaderTreeItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0, bool bWidthEditButton = false);

    //! Destructeur
    virtual ~CEditShaderTreeItem();

    //! Définit le chemin du fichier
    void setName(const QString& filePathName);

    //! Retourne le chemin du fichier
    QString getName() const;
};


#endif // CEDITSHADERTREEITEM_H
