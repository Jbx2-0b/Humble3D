#include "FormatConverterApp.h"
#include "CAssimpImporter.h"
#include "CMeshManager.h"

// Qt
#include <QFileDialog>

#define TESTLOADER


//-----------------------------------------------------------------------------------------
FormatConverterApp::FormatConverterApp(QWidget* parent /* = 0*/)
    : QMainWindow(parent)
    , m_Settings(configFileName, QSettings::IniFormat)
{
    ui.setupUi(this);

    connect(ui.actionSelect_files,		SIGNAL(triggered()),	this, SLOT(onBrowseClicked()));
    connect(ui.actionSelect_directory,	SIGNAL(triggered()),	this, SLOT(onBrowseClicked()));
    connect(ui.convertButton,			SIGNAL(clicked()),		this, SLOT(onConvertClicked()));
}

//-----------------------------------------------------------------------------------------
void FormatConverterApp::onBrowseClicked()
{
    bool bIsFile = sender() == ui.actionSelect_files;

    m_FilePathNameList.clear();

    ui.listWidget->clear();

    if (bIsFile)
    {
        QString inDirectory = m_Settings.value("InDirectory", QCoreApplication::applicationDirPath()).toString();
        m_FilePathNameList << QFileDialog::getOpenFileNames(this, tr("Open 3D Model"), inDirectory, tr("*.*"));

        if (!m_FilePathNameList.isEmpty())
        {
            m_Settings.setValue("InDirectory", QFileInfo(m_FilePathNameList[0]).canonicalPath());
        }
    }
    else // Directory
    {
        QString inDirectory = m_Settings.value("InDirectory", QCoreApplication::applicationDirPath()).toString();
        QString strPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), inDirectory);

        if (!strPath.isEmpty())
        {
            m_Settings.setValue("InDirectory", QFileInfo(strPath).canonicalPath());
            QStringList listFilter;
            listFilter << "*.*";

            QDirIterator dirIterator(strPath, listFilter, QDir::Files);

            while (dirIterator.hasNext())
            {
                m_FilePathNameList << dirIterator.next();
            }
        }
    }

    for (const QString& filePathName : m_FilePathNameList)
    {
        ui.listWidget->addItem(filePathName);
    }
}

//-----------------------------------------------------------------------------------------
void FormatConverterApp::onConvertClicked()
{
    ui.progressBar->setValue(0);

    int iFileCount = m_FilePathNameList.count();
    int i = 0;

    for (const QString& filePathName : m_FilePathNameList)
    {
        QFileInfo fileInfo(filePathName);

        m_SceneManager.clearAll();

        CSceneNode* pNode = m_SceneManager.getRootNode()->createChild(fileInfo.baseName());

        QTime time;
        time.start();
        if (!CAssimpImporter::mergeScene(filePathName, &m_SceneManager, false, pNode).isEmpty())
        {
            qDebug() << "Loaded node tree: " << fileInfo.baseName();
            pNode->dumpNodeTree();

            if (ui.resizeModelsCheckBox->isChecked())
            {
                // On récupére la bounding box
                CBox3D bbox = pNode->getGlobalAxisAlignedBoundingBox();

                real sizeX = bbox.getMaximum().x() - bbox.getMinimum().x();
                real sizeY = bbox.getMaximum().y() - bbox.getMinimum().y();
                real sizeZ = bbox.getMaximum().z() - bbox.getMinimum().z();

                real maxSize = sizeX;
                if (sizeY > maxSize) maxSize = sizeY;
                if (sizeZ > maxSize) maxSize = sizeZ;

                // On redimensionne la scene
                pNode->scale(1. / maxSize);

                CBox3D scaledBbox = pNode->getGlobalAxisAlignedBoundingBox();

                pNode->translate(-scaledBbox.getCenter());
            }

            QString newFilePathName = fileInfo.absolutePath() + "/" + QFileInfo(filePathName).baseName() + ".meshgroup";
            CMeshManager::getInstance().saveMeshGroup(CMeshManager::getInstance().getMeshs(), newFilePathName, ui.compressionSpinBox->value());

#ifdef TESTLOADER
            // test
            QFileInfo fileInfo1(newFilePathName);
            int iSize1 = fileInfo1.size();

            CMeshGroup meshGroupCopy = CMeshManager::getInstance().loadMeshGroup(newFilePathName);
            CMeshManager::getInstance().saveMeshGroup(meshGroupCopy, newFilePathName, ui.compressionSpinBox->value());
            QFileInfo fileInfo2(newFilePathName);
            int iSize2 = fileInfo2.size();

            Q_ASSERT(iSize1 == iSize2);
#endif
        }
        else
        {
            qDebug() << "Not readable by Assimp: " << fileInfo.baseName();
        }

        ui.progressBar->setValue(++i / (double) iFileCount * 100.);
    }
}
