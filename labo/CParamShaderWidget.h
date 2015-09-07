#ifndef QPARAMSHADERWIDGET_H
#define QPARAMSHADERWIDGET_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class CParamShaderWidget : public QWidget
{
	Q_OBJECT

public:

	//! Constructeur
	CParamShaderWidget(QWidget *parent = 0);

	//! Destructeur
	virtual ~CParamShaderWidget();

	//! Retourne le chemin du fichier
	QString getFilePathName() const { return m_pFilePathNameLineEdit->text(); }

	//! Définit le chemin du fichier
	void setFilePathName(const QString& filePathName) { m_pFilePathNameLineEdit->setText(filePathName); }

signals:

	void updateText();
	void saveFile();
	
private:

	QHBoxLayout* m_pHorizontalLayout;
	QLineEdit* m_pFilePathNameLineEdit;
	QPushButton* m_pFilePathNameSelectorButton;
	
private slots:

	void onFilePathNameSelectorButtonClicked();
};

#endif // QPARAMSHADERWIDGET_H
