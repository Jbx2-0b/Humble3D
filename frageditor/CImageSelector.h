#ifndef CIMAGESELECTOR_H
#define CIMAGESELECTOR_H

// Qt
#include <QWidget>

// UI
#include "ui_CImageSelector.h"

// Foundations
#include "CMaterial.h"
#include "CShader.h"

class CImageSelector : public QWidget
{
	Q_OBJECT

public:
	CImageSelector(QWidget *parent = 0);

	virtual ~CImageSelector();

	void setMaterial(CMaterial* pMaterial) { m_pMaterial = pMaterial; }
	
	void setShader(CShader* pShader) { m_pShader = pShader; }

protected:

	virtual void mousePressEvent(QMouseEvent* pEvent);

private:
	Ui::CImageSelector ui;

	QVector<QLabel*> channelLabels;

	CMaterial* m_pMaterial;

	CShader* m_pShader;
};

#endif // CIMAGESELECTOR_H
