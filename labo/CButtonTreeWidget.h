#ifndef CBUTTONTREEWIDGET_H
#define CBUTTONTREEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class CButtonTreeWidget : public QWidget
{
	Q_OBJECT

public:

	//! Constructeur
	CButtonTreeWidget(const QIcon& icon, QWidget *parent = 0);

	//! Destructeur
	virtual ~CButtonTreeWidget() {}

	//! Définit si le bouton est visible
	void setButtonVisible(bool bVisible)	{ m_pButton->setVisible(bVisible); }

	//! Retourne le texte
	QString getText() const					{ return m_pTextLabel->text(); }

	//! Définit le texte
	void setText(const QString& text)		{ m_pTextLabel->setText(text); }

	//! Définit l'îcone
	void setIcon(const QIcon& icon)			{ m_pButton->setIcon(icon); }


signals:

	void buttonClicked();

private:

	QHBoxLayout* m_pHorizontalLayout;
	QLabel* m_pTextLabel;
	QPushButton* m_pButton;
};

#endif // CBUTTONTREEWIDGET_H
