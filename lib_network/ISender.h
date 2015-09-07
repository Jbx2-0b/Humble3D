#ifndef ISENDER_H
#define ISENDER_H

// QT
#include <QObject>

//! D�finit la taille maximum d'un paquet
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

	//! D�finit le taux de compression (de 0 -pas de de compression- � 9)
	void setCompressionFactor(int iFactor)	{ m_iCompressionFactor = iFactor; }

signals:

	void newMessage(const QVariant& variant);

	void percentReceived(double dPercent);

protected:

	QString			m_strFilePath;
	int				m_iCompressionFactor;
};

#endif // ISENDER_H
