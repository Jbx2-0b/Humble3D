#include "CEditShaderTreeWidget.h"
#include "CParamShaderWidget.h"
#include "CShader.h"

//-----------------------------------------------------------------------------------------
CEditShaderTreeWidget::CEditShaderTreeWidget(QWidget *parent /*= 0*/)
: CButtonTreeWidget(QIcon(":/Resources/Edit2.png"), parent)
{
	connect(this, SIGNAL(buttonClicked()), this, SLOT(onEditClicked()));
}

//-----------------------------------------------------------------------------------------
CEditShaderTreeWidget::~CEditShaderTreeWidget()
{

}

//-----------------------------------------------------------------------------------------
void CEditShaderTreeWidget::onEditClicked()
{
	emit editShader(getText());
}
