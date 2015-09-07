#include "CExclusiveButton.h"

//-------------------------------------------------------------------------------------------------
CExclusiveButton::CExclusiveButton()
{
}

//-------------------------------------------------------------------------------------------------
CExclusiveButton::~CExclusiveButton()
{
}

//-------------------------------------------------------------------------------------------------
void CExclusiveButton::addButton(QPushButton* pPushButton)
{
    m_ButtonList.append(pPushButton);
    connect(pPushButton, SIGNAL(clicked()), this, SLOT(update()));
}

//-------------------------------------------------------------------------------------------------
void CExclusiveButton::update()
{
    if (QPushButton* pSender = dynamic_cast<QPushButton*>(sender()))
    {
        foreach (QPushButton* pPushButton, m_ButtonList)
        {
            if (pPushButton != pSender)
            {
                pPushButton->setChecked(false);
            }
        }

        pSender->setChecked(true);
    }
}	


