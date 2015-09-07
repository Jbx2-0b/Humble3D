#ifndef CIMAGESELECTORITEM_H
#define CIMAGESELECTORITEM_H


// Foundations
#include "CGraphicsWidgetItem.h"
#include "CImageSelector.h"
#include "CMaterial.h"

class CShader;

class CImageSelectorItem : public CGraphicsWidgetItem
{
public:
	
	CImageSelectorItem(QGraphicsScene* pScene, CMaterial* pMaterial, CShader* pShader)
		: CGraphicsWidgetItem(pScene)
	{
		m_pImgSelectorWidget = new CImageSelector();
		m_pImgSelectorWidget->setMaterial(pMaterial);
		m_pImgSelectorWidget->setShader(pShader);
		setWidget(m_pImgSelectorWidget);
	}

	//! Definit la feuille de style CSS
	void setStyleSheet(const QString& styleSheet)
	{
		m_pImgSelectorWidget->setStyleSheet(styleSheet);
	}


private:

	CImageSelector* m_pImgSelectorWidget;
};

#endif // CIMAGESELECTORITEM_H