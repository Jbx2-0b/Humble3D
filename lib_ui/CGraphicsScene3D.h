#ifndef CGRAPHICSSCENE3D_H
#define CGRAPHICSSCENE3D_H

// App
#include "CGLRenderer.h"
#include "CFPSManager.h"

//Qt
#include <QGraphicsScene>
#include <QTime>

class CGraphicsScene3D : public QGraphicsScene
{
	Q_OBJECT

public:

	//! Constructeur.
    CGraphicsScene3D(CFPSManager* pFPSManager, QObject* pParent = 0);

	//! Destructeur.
    virtual ~CGraphicsScene3D();

	//! Change le moteur de rendu
	void setRenderer(CGLRenderer* pRenderer);

protected:

	//! Rendu du fond
	void drawBackground(QPainter *painter, const QRectF &rect);

protected slots:

    void onTimeout();

private:

	//! Couleur de fond
	QColor m_BackgroundColor;
	
	//! Gestionnaire de rendu
	CGLRenderer* m_pRenderer;

	CFPSManager* m_pFPSManager;
};


#endif //CGRAPHICSSCENE3D_H
