#include "CButtonTreeWidget.h"

//-----------------------------------------------------------------------------------------
CButtonTreeWidget::CButtonTreeWidget(const QIcon& icon, QWidget *parent /*= 0*/)
    : QWidget(parent)
{
    m_pHorizontalLayout = new QHBoxLayout(this);
    m_pHorizontalLayout->setSpacing(1);
    m_pHorizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_pTextLabel = new QLabel(this);

    m_pHorizontalLayout->addWidget(m_pTextLabel);

    m_pButton = new QPushButton(this);
    m_pButton->setMinimumSize(QSize(20, 20));
    m_pButton->setMaximumSize(QSize(20, 20));
    m_pButton->setIcon(icon);
    m_pButton->setIconSize(QSize(16, 16));

    m_pHorizontalLayout->addWidget(m_pButton);

    connect(m_pButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
}
