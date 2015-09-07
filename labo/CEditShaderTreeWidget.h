#ifndef CEDITSHADERTREEWIDGET_H
#define CEDITSHADERTREEWIDGET_H

// Foundations
#include "CButtonTreeWidget.h"


class CEditShaderTreeWidget : public CButtonTreeWidget
{
	Q_OBJECT

public:

	//! Constructeur
	CEditShaderTreeWidget(QWidget *parent = 0);

	//! Destructeur
	virtual ~CEditShaderTreeWidget();

signals:

	void editShader(const QString& name);

private slots:

	void onEditClicked();
};

#endif // CEDITSHADERTREEWIDGET_H
