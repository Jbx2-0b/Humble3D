#include "CImageSelector.h"
#include "CTextureManager.h"

#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>

//-----------------------------------------------------------------------------------------
CImageSelector::CImageSelector(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    channelLabels
            << ui.channel0Label
            << ui.channel1Label
            << ui.channel2Label
            << ui.channel3Label;

}

//-----------------------------------------------------------------------------------------
CImageSelector::~CImageSelector()
{

}

//-----------------------------------------------------------------------------------------
void CImageSelector::mousePressEvent(QMouseEvent* pEvent)
{
    int iID = -1;

    for (int i = 0; i < channelLabels.size(); ++i)
    {
        if (channelLabels[i]->geometry().contains(pEvent->pos()))
            iID = i;
    }

    if (iID >= 0)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), QCoreApplication::applicationDirPath(), tr("*.*"));

        if (!fileName.isEmpty())
        {
            QPixmap base(fileName);

            if (!base.isNull())
            {
                QPixmap pix = base.scaled(channelLabels[iID]->size());
                channelLabels[iID]->setPixmap(pix);

                if (CTexture2D* pTexture = dynamic_cast<CTexture2D*>(CTextureManager::getInstance()->getTextureByName(m_pMaterial->getTextureParams()[iID].getTextureName())))
                {
                    pTexture->setFileName(fileName);
                    m_pShader->setUniformValue(QString("iChannelResolution[%1]").arg(iID), QVector3D(pTexture->getSize().width(), pTexture->getSize().height(), 0.));
                }
            }
        }
    }

    // on remonte les evenements
    QWidget::mousePressEvent(pEvent);
}
