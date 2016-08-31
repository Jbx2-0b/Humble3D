#include "CAnimationTreeWidget.h"

//-----------------------------------------------------------------------------------------
CAnimationTreeWidget::CAnimationTreeWidget(QWidget *parent /*= 0*/)
    : CButtonTreeWidget(QIcon(":/Resources/Run.png"), parent)
    , m_bPlay(false)
{
    connect(this, SIGNAL(buttonClicked()), this, SLOT(onPlayClicked()));
}

//-----------------------------------------------------------------------------------------
CAnimationTreeWidget::~CAnimationTreeWidget()
{

}

//-----------------------------------------------------------------------------------------
void CAnimationTreeWidget::onPlayClicked()
{
    m_bPlay = !m_bPlay;
    setIcon(QIcon(m_bPlay ? ":/Resources/Quit.png" : ":/Resources/Run.png"));

    emit playAnimation(getText(), m_bPlay);
}
