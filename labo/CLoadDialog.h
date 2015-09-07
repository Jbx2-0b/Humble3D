#ifndef CLOADDIALOG_H
#define CLOADDIALOG_H

#include <QDialog>

#include "ui_CLoadDialog.h"

class CLoadDialog : public QDialog
{
    Q_OBJECT

public:
    CLoadDialog(QWidget *parent = 0);

    virtual ~CLoadDialog();

    bool mergeMaterials() const		{ return ui.mergeMaterialsCheckBox->isChecked(); }
    bool rescaleMeshs() const		{ return ui.rescaleMeshsCheckBox->isChecked(); }
    bool invertXZ() const			{ return ui.invertXZCheckBox->isChecked(); }
    bool clearEmptyNodes() const	{ return ui.emptyNodesCheckBox->isChecked(); }
    bool clearMeshs() const			{ return ui.meshsCheckBox->isChecked(); }
    bool clearAnimations() const	{ return ui.animationsCheckBox->isChecked(); }
    bool clearLights() const		{ return ui.lightsCheckBox->isChecked(); }
    bool clearMaterials() const		{ return ui.materialsCheckBox->isChecked(); }
    bool clearCameras() const		{ return ui.camerasCheckBox->isChecked(); }
    bool clearShaders() const		{ return ui.shadersCheckBox->isChecked(); }


private:

    Ui::CLoadDialog ui;
};

#endif // CLOADDIALOG_H
