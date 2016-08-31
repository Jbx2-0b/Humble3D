#ifndef CANIMATIONTREEWIDGET_H
#define CANIMATIONTREEWIDGET_H

#include "CButtonTreeWidget.h"

class CAnimationTreeWidget : public CButtonTreeWidget
{
    Q_OBJECT

public:

    CAnimationTreeWidget(QWidget *parent = 0);

    virtual ~CAnimationTreeWidget();

signals:

    void playAnimation(const QString& name, bool bPlay);

private slots:

    void onPlayClicked();

private:

    bool m_bPlay;

};

#endif // CANIMATIONTREEWIDGET_H
