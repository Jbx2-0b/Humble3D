#ifndef ISENDER_H
#define ISENDER_H

// QT
#include <QObject>

//! Définit la taille maximum d'un paquet
typedef quint32 MaxPacketSize;

class ISender : public QObject
{
	Q_OBJECT

	friend class CTypeDispatcher;

public:

	//! Ctor
	ISender(QObject* parent = 0) 
		: QObject(parent)
		, m_iCompressionFactor(0)
		{}

	//! Définit le taux de compression (de 0 -pas de de compression- à 9)
	void setCompressionFactor(int iFactor)	{ m_iCompressionFactor = iFactor; }

signals:

	void newMessage(const QVariant& variant);

	void percentReceived(double dPercent);

protected:

	QString			m_strFilePath;
	int				m_iCompressionFactor;
};

#endif // ISENDER_H
