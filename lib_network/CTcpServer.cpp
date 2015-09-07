
// Qt
#include <QFile>
#include <QCoreApplication>
#include <QTimer>
#include <QTcpSocket>
#include <QDataStream>

// Fondations
#include "CLogManager.h"

// Applications
#include "CTcpServer.h"

//-------------------------------------------------------------------------------------------------
CTcpServer::CTcpServer(QObject *parent /*= 0*/)
    : ISender(parent)
    , m_Mutex(QMutex::Recursive)
    , m_iListenPort(0)
    , m_bIsRunning(false)
    , m_iBlockSize(0)
    , m_bLastMessageNotSend(false)
{
    connect(&m_Server,	SIGNAL(newConnection()),		this, SLOT(onServerConnection()));
    connect(this,		SIGNAL(doSendMessage(QString)), this, SLOT(onSendMessage(QString)));
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::stop()
{
    m_bIsRunning = false;

    m_Server.close();

    foreach (QTcpSocket* tcpSocket, m_TcpSocketList)
    {
        tcpSocket->close();
    }

    m_TcpSocketList.clear();
}

//-------------------------------------------------------------------------------------------------
bool CTcpServer::sendMessage(const QVariant& message, const QHostAddress& address)
{
    m_SendMessage = message;

    emit doSendMessage(address.toString());

    return !m_bLastMessageNotSend;
}

//-------------------------------------------------------------------------------------------------
bool CTcpServer::sendMessage(const QVariant& message)
{
    m_SendMessage = message;

    emit doSendMessage("");

    return !m_bLastMessageNotSend;
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::startListening(int iListenPort, QHostAddress address /*= QHostAddress::Any*/)
{
    m_iListenPort = iListenPort;

    if (!m_Server.listen(address, m_iListenPort))
    {
        QString sMsg = QString("CTcpServer::startListening() : Unable to start the server on  %1:%2 > %3").arg(address.toString()).arg(m_iListenPort).arg(m_Server.errorString());

        qDebug() << sMsg;

        m_bIsRunning = false;
    }
    else
    {
        m_bIsRunning = true;
    }
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::onSendMessage(const QString& strIP /*= ""*/)
{
    if (m_bIsRunning)
    {
        if (m_SendMessage.isValid())
        {
            if (!m_TcpSocketList.isEmpty())
            {
                foreach (QTcpSocket* tcpSocket, m_TcpSocketList)
                {
                    if (tcpSocket->isValid())
                    {
                        m_bLastMessageNotSend = false;

                        // Si l'adresse IP est vide on transmet e tout les clients
                        if ((!strIP.isEmpty() && tcpSocket->peerAddress().toString() == strIP) || strIP.isEmpty())
                        {
                            QByteArray packet;

                            // Pour ne pas surchager les donnees en sortie du serveur,
                            // On verifie le buffer sortant avec bytesToWrite()
                            if (tcpSocket->bytesToWrite() < (1024 * 1024))
                            {
                                QDataStream out(&packet, QIODevice::WriteOnly);

                                out.setVersion(QDataStream::Qt_4_7);
                                out << (MaxPacketSize)0;
                                out << 	m_SendMessage;
                                out.device()->seek(0);
                                out << (MaxPacketSize)(packet.size() - sizeof(MaxPacketSize));

                                tcpSocket->write(packet);
                            }
                        }
                    }
                }
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
    else
    {
        m_bLastMessageNotSend = true;
    }
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::onServerConnection()
{
    QTcpSocket* pSocket = m_Server.nextPendingConnection();

    // On verifie qu'il n'existe pas deje un client avec la meme adresse IP
    // Si c'est le cas on la supprime
    remove(pSocket->peerAddress());

    m_TcpSocketList.append(pSocket);

    connect(pSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(pSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    emit connected(pSocket->peerAddress().toString());
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::onSocketReadyRead()
{
    if (QTcpSocket* pSocket = dynamic_cast<QTcpSocket*>(QObject::sender()))
    {
        QDataStream in(pSocket);
        in.setVersion(QDataStream::Qt_4_7);

        do
        {
            if (m_iBlockSize == 0)
            {
                if (pSocket->bytesAvailable() < (int)sizeof(MaxPacketSize))
                    return;

                in >> m_iBlockSize;
            }

            if (pSocket->bytesAvailable() < m_iBlockSize)
            {
                emit percentReceived(pSocket->bytesAvailable() / (double)m_iBlockSize);
                return;
            }

            emit percentReceived(100.);

            m_iBlockSize = 0;

            QVariant lastMsg;
            in >> lastMsg;

            emit newMessage(lastMsg);
        }
        while (pSocket->bytesAvailable() > 0);
    }
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::onDisconnected()
{
    if (QTcpSocket* pSocket = dynamic_cast<QTcpSocket*>(QObject::sender()))
    {
        emit disconnected(pSocket->peerAddress().toString());

        remove(pSocket->peerAddress());
    }
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::onSocketError(QAbstractSocket::SocketError /*error*/)
{
    if (QTcpSocket* pSocket = dynamic_cast<QTcpSocket*>(QObject::sender()))
    {
        emit connectionError(pSocket->peerAddress().toString(), pSocket->errorString());
        emit disconnected(pSocket->peerAddress().toString());

        pSocket->disconnect();
    }
}

//-------------------------------------------------------------------------------------------------
void CTcpServer::remove(const QHostAddress& peerAddress)
{
    for (int iIndex = 0; iIndex < m_TcpSocketList.count(); iIndex++)
    {
        if (m_TcpSocketList[iIndex]->peerAddress() == peerAddress)
        {
            m_TcpSocketList[iIndex]->disconnect();
            m_TcpSocketList[iIndex]->disconnectFromHost();
            m_TcpSocketList[iIndex]->deleteLater();
            m_TcpSocketList.remove(iIndex);
            iIndex--;
        }
    }
}
