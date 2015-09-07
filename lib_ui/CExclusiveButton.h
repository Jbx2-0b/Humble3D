#ifndef CEXCLUSIVEBUTTON_H
#define CEXCLUSIVEBUTTON_H

// Lib
#include "lib_ui_global.h"

// Qt
#include <QObject>
#include <QList>
#include <QPushButton>

class LIB_UI_SHARED_EXPORT CExclusiveButton : public QObject
{
	Q_OBJECT

public:

	//! Ctor
	CExclusiveButton();
	
	//! Dtor
	virtual ~CExclusiveButton();

	//! Ajouter un bouton
	void addButton(QPushButton* pPushButton);

protected slots:

	void update();
	
private:

	QList<QPushButton*> m_ButtonList;
};


#endif // EXCLUSIVEBUTTON_H
