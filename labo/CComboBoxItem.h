

#ifndef CCOMBOBOXITEM_H
#define CCOMBOBOXITEM_H

// Foundations
#include "CTreeWidgetItemBase.h"

// Qt
#include <QComboBox>

class CComboBoxItem : public CTreeWidgetItemBase<QComboBox>
{
	Q_OBJECT

public:

	//! Constructeur
	CComboBoxItem(CNodeTreeWidget* pTreeWidget, QTreeWidgetItem* pParent = 0);

	//! Destructeur
	virtual ~CComboBoxItem();

	//! Ajoute une liste d'entr�e dans la comboBox
	void addEntries(const QList<QString>& entries);

	//! Ajoute une entr�e dans la comboBox
	void addEntry(const QString& entry);

	//! Supprime une entr�e dans la comboBox
	void removeEntry(const QString& entry);

	//! Retourne l'entr�e courante de la comboBox
	QString getCurrentEntry() const;

	//! D�finit l'entr�e courante de la comboBox
	void setCurrentEntry(const QString& entry);

private slots:
	
	void onCurrentIndexChanged(int);

};

#endif // CCOMBOBOXITEM_H
