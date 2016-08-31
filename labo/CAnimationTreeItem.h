#ifndef CANIMATIONTREEITEM_H
#define CANIMATIONTREEITEM_H

// Foundations
#include "CTreeWidgetItemBase.h"
#include "CAnimationTreeWidget.h"

class CAnimationTreeItem : public CTreeWidgetItemBase<CAnimationTreeWidget>
{
    Q_OBJECT

public:

    //! Constructeur
    CAnimationTreeItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0);

    //! Destructeur
    virtual ~CAnimationTreeItem();

    //! Definit le chemin du fichier
    void setName(const QString& filePathName);

    //! Retourne le chemin du fichier
    QString getName() const;

};

#endif // CANIMATIONTREEITEM_H
