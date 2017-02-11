#include "ScriptLoaderApp.h"
#include "CScriptManager.h"

#include <QFileDialog>
#include <QTime>
#include <QDebug>

ScriptLoaderApp::ScriptLoaderApp()
{
    // On crée une camera
    CCamera* pCamera = getSceneManager()->createCamera();

    pCamera->setEyePosition(QVector3D(-3.26, 2.0, -0.03));
    pCamera->setCenter(QVector3D(0., 0., 0.));

    CWidget3D* pView = createWidget3D(pCamera);
    m_pController = new Controller(pView);
    pView->setForceUpdate(true);
    pView->getFPSManager()->setDesiredFPS(1000);

    QString fileName = QFileDialog::getOpenFileName(0, "Open Script", QCoreApplication::applicationDirPath(), "*.h3d");

    if (!fileName.isEmpty())
    {
        CScriptManager sManager(getSceneManager());
        sManager.loadScript(fileName, getSceneManager()->getRootNode());
    }

    //getSceneManager()->buildOctree(CBox3D(QVector3D(-50, -50, -50), QVector3D(+50, +50, +50)), 1000, 5);

    qDebug() << "World Bounding Box: " << getSceneManager()->getWorldAxisAlignedBoundingBox().toString();
}

ScriptLoaderApp::~ScriptLoaderApp()
{
    delete m_pController;
}
