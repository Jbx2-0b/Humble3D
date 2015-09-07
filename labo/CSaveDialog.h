#ifndef CSAVEDIALOG_H
#define CSAVEDIALOG_H

#include <QDialog>
#include "ui_CSaveDialog.h"

class CSaveDialog : public QDialog
{
    Q_OBJECT

public:
    CSaveDialog(QWidget *parent = 0);

    virtual ~CSaveDialog();

    bool saveAnimations() const			{ return ui.animationsCheckBox->isChecked(); }
    bool saveLights() const				{ return ui.lightsCheckBox->isChecked(); }
    bool saveMaterials() const			{ return ui.materialsCheckBox->isChecked(); }
    bool saveCameras() const			{ return ui.camerasCheckBox->isChecked(); }
    bool saveShaders() const			{ return ui.shadersCheckBox->isChecked(); }

    int compressionFactor() const		{ return ui.compressionSpinBox->value(); }

private:

    Ui::CSaveDialog ui;
};

#endif // CSAVEDIALOG_H
