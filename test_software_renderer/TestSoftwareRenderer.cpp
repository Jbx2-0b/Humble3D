#include "TestSoftwareRenderer.h"
#include "CAssimpImporter.h"
#include "CSoftwareView3D.h"
#include "CAnimationManager.h"

#include "Math.h"

//-----------------------------------------------------------------------------------------
TestSoftwareRenderer::TestSoftwareRenderer()
{
    // On crée une camera
    CCamera* pCamera = getSceneManager()->createCamera();
    pCamera->setEyePosition(QVector3D(1., 1., 1.));
    pCamera->setCenter(QVector3D(0., 0.5, 0.));

    CSoftwareView3D* pView = createSoftwareView3D(pCamera);

    pCamera->setAspectRatio((real)pView->width() / (real)pView->height());

    // On charge le modéle
    CSceneNode* pRootNode = m_pSceneManager->getRootNode();

    CSceneNode* pModelNode = pRootNode->createChild("dwarf");

    // anim_test.x
    // rotatingcube.3ds
    // dwarf.x
    if (!CAssimpImporter::mergeScene("://models/dwarf.x", m_pSceneManager, true, pModelNode).isEmpty())
    {
        // On récupére la bounding box
        CBox3D bbox = pRootNode->getGlobalAxisAlignedBoundingBox();

        real sizeX = bbox.getMaximum().x() - bbox.getMinimum().x();
        real sizeY = bbox.getMaximum().y() - bbox.getMinimum().y();
        real sizeZ = bbox.getMaximum().z() - bbox.getMinimum().z();

        real maxSize = sizeX;
        if (sizeY > maxSize) maxSize = sizeY;
        if (sizeZ > maxSize) maxSize = sizeZ;

        // On redimensionne la scene de façon à ce qu'elle ait une taille de 1x1x1 max
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

        pView->getRenderer()->setRasterizationMode(eRasterizationLine);

        //foreach (AMesh* pMesh, m_pSceneManager->getMeshs())
        //{
        //	pMesh->setSkeletonVisible(true);
        //}

        pRootNode->dumpNodeTree();
        //m_pSceneManager->setSceneGraphVisible(true);

        pView->setGeometry(QRect(100, 100, 400, 300));
        getAnimationManager()->run();

        QList<CAnimation*> anims = m_pSceneManager->getAnimations();

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
TestSoftwareRenderer::~TestSoftwareRenderer()
{

}
