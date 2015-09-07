
// Qt
#include <QtNetwork>

// Fondations
#include "CLogManager.h"

// Application
#include "CTcpClient.h"

//-------------------------------------------------------------------------------------------------
CTcpClient::CTcpClient(int iPort /*= 0*/, QObject *parent /*= 0*/)
: ISender(parent)
, m_iBlockSize(0)
, m_iPort(iPort)
, m_bLastMessageNotSend(false)
{
	m_TcpSocket = new QTcpSocket(this);
	
	connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
	connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	connect(m_TcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
	
	connect(&m_RetryConnectionTimer, SIGNAL(timeout()), this, SLOT(reconnectToHost()));
}

//-------------------------------------------------------------------------------------------------
CTcpClient::~CTcpClient()
{
	disconnectFromHost();
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::setPort(int iPort)
{
	m_iPort = iPort;
}

//-------------------------------------------------------------------------------------------------
bool CTcpClient::isConnected()
{
	return m_TcpSocket->state() == QAbstractSocket::ConnectedState;
}

//-------------------------------------------------------------------------------------------------
QAbstractSocket::SocketState CTcpClient::getState() const
{
	return m_TcpSocket->state();	
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::connectToHost(const QString& strHostIPAdress)
{
	// L'adresse IP différe, on se deconnecte de l'éventuel précédent serveur
	if (!m_strHostIPAdress.isEmpty() && m_strHostIPAdress != strHostIPAdress)
	{
		disconnectFromHost();
		m_strHostIPAdress = strHostIPAdress;
	}

	if (!isConnected())
	{
		m_iBlockSize = 0;
		m_strHostIPAdress = strHostIPAdress;
		m_TcpSocket->connectToHost(m_strHostIPAdress, m_iPort, QIODevice::ReadWrite);
		m_RetryConnectionTimer.start(iDTReconnection);
	}
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::reconnectToHost()
{
	if (!isConnected())
	{
		m_iBlockSize = 0;
		m_TcpSocket->connectToHost(m_strHostIPAdress, m_iPort, QIODevice::ReadWrite);
		m_RetryConnectionTimer.start(iDTReconnection);
	}
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::disconnectFromHost()
{
	if (isConnected())
	{
		m_TcpSocket->disconnectFromHost();
	}

	m_RetryConnectionTimer.stop();
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::onSocketReadyRead()
{
	QDataStream in(m_TcpSocket);
	in.setVersion(QDataStream::Qt_4_7);

	do
	{
		if (m_iBlockSize == 0)
		{
			if (m_TcpSocket->bytesAvailable() < (int)sizeof(MaxPacketSize))
				return;

			in >> m_iBlockSize;
		}

		if (m_TcpSocket->bytesAvailable() < m_iBlockSize)
		{
			emit percentReceived(m_TcpSocket->bytesAvailable() / (double)m_iBlockSize);
			return;
		}

		emit percentReceived(100.);

		m_iBlockSize = 0;

		QVariant lastMsg;
		in >> lastMsg;

		emit newMessage(lastMsg);
	}
	while (m_TcpSocket->bytesAvailable() > 0);
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::onSocketError(QAbstractSocket::SocketError /*socketError*/)
{
	emit connectionError(m_TcpSocket->errorString());
}

//-------------------------------------------------------------------------------------------------
bool CTcpClient::sendMessage(const QVariant& message)
{
	m_SendMessage = message;
	
	doSendMessage();

	return !m_bLastMessageNotSend;
}

//-------------------------------------------------------------------------------------------------
void CTcpClient::doSendMessage()
{
	if (isConnected())
	{
		if (m_SendMessage.isValid())
		{
			m_bLastMessageNotSend = false;

			QByteArray packet;

			QDataStream out(&packet, QIODevice::WriteOnly);
			out.setVersion(QDataStream::Qt_4_7);
			out << (MaxPacketSize)0;
			out << m_SendMessage;
			out.device()->seek(0);  
			out << (MaxPacketSize)(packet.size() - sizeof(MaxPacketSize));

			m_TcpSocket->write(packet);
		}
		else
		{
			m_bLastMessageNotSend = true;
		}
	}
	else
	{
		m_bLastMessageNotSend = true;
	}
}
