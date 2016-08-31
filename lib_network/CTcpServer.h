#ifndef CTCPSERVER_H
#define CTCPSERVER_H

// Lib
#include "lib_network_global.h"

// Qt
#include <QTcpServer>
#include <QList>
#include <QMutex>

// Application
#include "ISender.h"

class LIB_NETWORKSHARED_EXPORT CTcpServer : public ISender
{
    Q_OBJECT

public:

    //! Ctor
    CTcpServer(QObject *parent = 0);

    //! Démarre l'écoute du serveur
    void startListening(int iListenPort, QHostAddress address = QHostAddress::Any);

    //! Retourne true si des clients sont connectes
    bool hasConnectedClients() const { return m_TcpSocketList.count() > 0; }

signals:

    //! Signal émit sur connection d'un client
    void connected(const QString& strIP);

    //! Signal émit sur déconnection d'un client
    void disconnected(const QString& strIP);

    //! Signal émit sur erreur d'un client
    void connectionError(const QString& strIP, const QString& strError);

    //! Signal utilisé en interne pour être sur d'être sur le bon thread
    void doSendMessage(const QString& strIP);

public slots:

    //! Arrête le serveur
    void stop();

    bool sendMessage(const QVariant& message, const QHostAddress& address);
    bool sendMessage(const QVariant& message);

protected slots:

    void onServerConnection();
    void onSocketReadyRead();
    void onDisconnected();
    void onSendMessage(const QString& strIP = "");
    void onSocketError(QAbstractSocket::SocketError error);

protected:

    QMutex					m_Mutex;
    QTcpServer				m_Server;
    QVariant				m_SendMessage;
    int						m_iListenPort;
    QVector<QTcpSocket*>	m_TcpSocketList;
    bool					m_bIsRunning;
    MaxPacketSize			m_iBlockSize;

    void remove(const QHostAddress& peerAddress);

    bool m_bLastMessageNotSend;
};

#endif // CTCPSERVER_H
