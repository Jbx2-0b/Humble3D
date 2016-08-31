#include "TestAnimation.h"
#include "CResourcesPathManager.h"
#include "CMaterialManager.h"
#include "CAssimpImporter.h"
#include "CGLRenderer.h"
#include "CWidget3D.h"
#include "CAnimationManager.h"

//-----------------------------------------------------------------------------------------
TestAnimation::TestAnimation()
{
    // On crée une camera
    m_pCamera = getSceneManager()->createCamera();
    m_pCamera->setEyePosition(QVector3D(2., 1., 2.));
    m_pCamera->setCenter(QVector3D(0., 0., 0.));


#ifdef SOFTWARE_RENDERING
    m_pView = createSoftwareView3D(m_pCamera);
#else
    m_pView = createWidget3D(m_pCamera);
#endif

    m_pCamera->setAspectRatio((real)m_pView->width() / (real)m_pView->height());

    // On charge le modéle
    CSceneNode* pRootNode = m_pSceneManager->getRootNode();

    CSceneNode* pModelNode = pRootNode->createChild("dwarf");

    // anim_test.x
    // rotatingcube.3ds
    // dwarf.x

    if (!CAssimpImporter::mergeScene("://dwarf.x", m_pSceneManager, true, pModelNode).isEmpty())
    {
        // On récupére la bounding box
        CBox3D bbox = pRootNode->getGlobalAxisAlignedBoundingBox();

        real sizeX = bbox.getMaximum().x() - bbox.getMinimum().x();
        real sizeY = bbox.getMaximum().y() - bbox.getMinimum().y();
        real sizeZ = bbox.getMaximum().z() - bbox.getMinimum().z();

        real maxSize = sizeX;
        if (sizeY > maxSize) maxSize = sizeY;
        if (sizeZ > maxSize) maxSize = sizeZ;

        // On redimensionne la scene de façon à ce qu'elle tienne dans une boite de 1x1x1
        pModelNode->scale(1. / maxSize);

        CBox3D scaledBbox = pModelNode->getGlobalAxisAlignedBoundingBox();

        // On la centre
        pModelNode->translate(-scaledBbox.getCenter());

        CSceneNode* pLightNode = pRootNode->createChild("LightNode", QVector3D(8.0, 12.0, 0));

        // On crée une lumiére diffuse blanche
        CLight* pLight = m_pSceneManager->createLight("Light");
        pLight->setDiffuseColor(0.8f, 0.8f, 0.8f);
        pLight->setAmbientColor(0.6f, 0.6f, 0.6f);
        pLight->setSpecularColor(0.4f, 0.4f, 0.4f);
        pLight->setDirection(QVector3D(-1, -1, 0));

        // On l'associe au noeud
        pLightNode->addItem(pLight);

        pRootNode->dumpNodeTree();
        //m_pSceneManager->setSceneGraphVisible(true);

        QList<CAnimation*> anims = m_pSceneManager->getAnimations();

        m_pView->setGeometry(QRect(100, 100, 400, 300));
        m_pView->getRenderer()->run();
        getAnimationManager()->run();

        if (anims.size() > 0)
        {
            QList<CSceneNodeAnimation*> nodeAnimations = anims[0]->getNodeAnimations();

            foreach (CSceneNodeAnimation* pNodeAnim, nodeAnimations)
            {
                pNodeAnim->setAnimationBehavior(eAnimationBehaviourRepeat);
            }

            anims[0]->start();
        }
    }
}

//-----------------------------------------------------------------------------------------
TestAnimation::~TestAnimation()
{

}
