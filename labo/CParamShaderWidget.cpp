#include "CParamShaderWidget.h"
#include "CShader.h"

#include <QFileDialog>
#include <QCoreApplication>


//-----------------------------------------------------------------------------------------
CParamShaderWidget::CParamShaderWidget(QWidget *parent /*= 0*/)
: QWidget(parent)
{
	m_pHorizontalLayout = new QHBoxLayout(this);
	m_pHorizontalLayout->setSpacing(1);
	m_pHorizontalLayout->setContentsMargins(0, 0, 0, 0);
	
	m_pFilePathNameLineEdit = new QLineEdit(this);

	m_pHorizontalLayout->addWidget(m_pFilePathNameLineEdit);

	m_pFilePathNameSelectorButton = new QPushButton(this);
	m_pFilePathNameSelectorButton->setMinimumSize(QSize(20, 20));
	m_pFilePathNameSelectorButton->setMaximumSize(QSize(20, 20));
	m_pFilePathNameSelectorButton->setText("...");

	m_pHorizontalLayout->addWidget(m_pFilePathNameSelectorButton);

	connect(m_pFilePathNameSelectorButton,	SIGNAL(clicked()), this, SLOT(onFilePathNameSelectorButtonClicked()));
}

//-----------------------------------------------------------------------------------------
CParamShaderWidget::~CParamShaderWidget()
{

}

//-----------------------------------------------------------------------------------------
void CParamShaderWidget::onFilePathNameSelectorButtonClicked()
{
	QString filePathName = QFileDialog::getOpenFileName(this, tr("Open Shaders"), "", tr("Shaders Files (*.glsl)"));

	if (!filePathName.isEmpty())
	{
		m_pFilePathNameLineEdit->setText(filePathName);
	}
}
