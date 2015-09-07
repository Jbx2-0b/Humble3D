#ifndef QTHELPER_H
#define QTHELPER_H

// Qt
#include <QString>
#include <QFile>
#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QVector4D>

// Foundations
#include "CMath.h"
#include "CMouseStates.h"
#include "CKeyboardStates.h"


class CQtHelper
{
public:

	//-----------------------------------------------------------------------------------------
	// FEUILLE DE STYLE
	//-----------------------------------------------------------------------------------------

    static QString getStyleSheetFromFileName(const QString& fileName)
    {
        QString styleSheet;
        QFile styleFile(fileName);

        if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            styleSheet = QString::fromUtf8(styleFile.readAll());
            styleFile.close();
        }
        else
        {
            qDebug() << "Error opening: " << fileName;
        }


        return styleSheet;
    }

	//! fonction pour charger un fichier CSS et le donner sous forme de String
	static QString loadStyleSheet(const QString& filename)
	{
		QFile styleFile(filename);
		if (!styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			return QString();
		}
		else
		{
			return QString::fromUtf8(styleFile.readAll());
		}
	}

	//! Fonction pour convertir les QVector4D en QColor
	static QColor toQColor(const QVector4D& v)
	{
		return QColor(v.x() * 255., v.y() * 255., v.z() * 255., v.w() * 255.);
	}

    //! Fonction pour convertir les real en QColor
    static QColor toQColor(real color[4])
	{
		return QColor(color[0] * 255., color[1] * 255., color[2] * 255., color[3] * 255.);
	}

	//! Fonction pour convertir les QColor en QVector4D
	static QVector4D toColor(const QColor& v)
	{
		return QVector4D(v.redF() / 255., v.greenF() / 255., v.blueF() / 255., v.alphaF() / 255.);
	}

	static CMouseStates fromMouseEvent(QMouseEvent* pEvent)
	{
		CMouseStates states;
		states.setPosition(pEvent->pos());
		states.setGlobalPosition(pEvent->globalPos());
		states.setRightButtonPressed(pEvent->buttons() & Qt::RightButton);
		states.setMidButtonPressed(pEvent->buttons() & Qt::MidButton);
		states.setLeftButtonPressed(pEvent->buttons() & Qt::LeftButton);
		return states;
	}

	static CKeyboardStates fromQKeyEvent(QKeyEvent* pEvent)
	{
		CKeyboardStates states;
		states.setKey(pEvent->key());
		return states;
	}

};

#endif // QTHELPER_H
