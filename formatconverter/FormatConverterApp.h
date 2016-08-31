#ifndef FORMATCONVERTERAPP_H
#define FORMATCONVERTERAPP_H

#include <QMainWindow>
#include <QSettings>

#include "CSceneManager.h"
#include "CMaterialManager.h"
#include "ui_FormatConverter.h"

static const QString configFileName = "config.ini";

class FormatConverterApp : public QMainWindow
{
    Q_OBJECT

public:

    FormatConverterApp(QWidget *parent = 0);

    virtual ~FormatConverterApp()
    {
        CMaterialManager::killInstance();
    }

protected slots:

    void onBrowseClicked();
    void onConvertClicked();

private:

    CSceneManager m_SceneManager;
    QStringList m_FilePathNameList;
    Ui::MainWindow ui;
    QSettings m_Settings;

};

#endif // FORMATCONVERTERAPP_H
