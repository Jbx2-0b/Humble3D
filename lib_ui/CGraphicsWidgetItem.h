#ifndef CGRAPHICSWIDGETITEM_H
#define CGRAPHICSWIDGETITEM_H

// Lib
#include "lib_ui_global.h"


#include "CQtHelper.h"

// Qt
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QWidget>
#include <QFile>


class CGraphicsWidgetItem : public QGraphicsProxyWidget
{
public:

    //! Ctor
    CGraphicsWidgetItem(QGraphicsItem* parent = 0)
        : QGraphicsProxyWidget(parent) {}

    //! Ctor
    CGraphicsWidgetItem(QGraphicsScene* pScene)
    {
        pScene->addItem(this);
    }

    template <class Widget>
    Widget* getWidget() const
    {
        return dynamic_cast<Widget*>(widget());
    }

    //! Largeur
    int width()		{ return boundingRect().width(); }

    //! Hauteur
    int height()	{ return boundingRect().height(); }

    // Crée un item à partir d'un widget
    static CGraphicsWidgetItem* createItemFromWidget(QWidget* pWidget, QGraphicsItem* parent = 0)
    {
        CGraphicsWidgetItem* pItem = new CGraphicsWidgetItem(parent);
        pItem->setWidget(pWidget);
        pWidget->setAttribute(Qt::WA_TranslucentBackground);
        pItem->setZValue(1e30);
        return pItem;
    }

    void setStyleSheetFile(const QString& styleSheetFileName)
    {
        if (widget())
        {
            widget()->setStyleSheet(CQtHelper::getStyleSheetFromFileName(styleSheetFileName));
        }
    }
};

template <class Form>
class CGraphicsFormItem : public CGraphicsWidgetItem
{
public:
    //! Ctor
    CGraphicsFormItem(QGraphicsItem* parent = 0)
        : CGraphicsWidgetItem(parent)
        , m_pForm(0) {}

    //! Dtor
    ~CGraphicsFormItem()
    {
        delete m_pForm;
    }

    // Crée un item à partir d'un widget
    static CGraphicsFormItem* createItemFromFormAndWidget(Form* pForm, QWidget* pWidget, QGraphicsItem* parent = 0)
    {
        CGraphicsFormItem* pItem = new CGraphicsFormItem(parent);
        pItem->setWidget(pWidget);
        pWidget->setAttribute(Qt::WA_TranslucentBackground);
        pItem->setZValue(1e30);
        pItem->setForm(pForm);
        return pItem;
    }

    Form* getForm() { return m_pForm; }

    void setForm(Form* form) { m_pForm = form; }

protected:
    Form* m_pForm;
};

#endif // CGRAPHICSWIDGETITEM_H
