#ifndef CPARAMSHADERITEM_H
#define CPARAMSHADERITEM_H

// Foundations
#include "CTreeWidgetItemBase.h"
#include "CParamShaderWidget.h"

class CParamShaderItem : public CTreeWidgetItemBase<CParamShaderWidget>
{
	Q_OBJECT

public:
	//! Constructeur
	CParamShaderItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0, bool bWidthEditButton = false);

	//! Destructeur
	virtual ~CParamShaderItem();

	//! Definit le chemin du fichier
	void setFilePathName(const QString& filePathName);

	//! Retourne le chemin du fichier
	QString getFilePathName() const;
};

#endif // CPARAMSHADERITEM_H
