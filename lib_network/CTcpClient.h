#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

// Lib
#include "lib_network_global.h"

// Qt
#include <QTcpSocket>
#include <QTimer>

// Application
#include "ISender.h"

class LIB_NETWORKSHARED_EXPORT CTcpClient : public ISender
{
    Q_OBJECT

public:

    //! Ctor
    CTcpClient(int iPort = 0, QObject *parent = 0);

    //! Dtor
    virtual ~CTcpClient();

    //! Définit le port pour se connecter au serveur
    void setPort(int iPort);

    //! Transmet un objet
    bool sendMessage(const QVariant& message);

    //! Retourne vrai si est connecté
    bool isConnected();

    //! Retourne l'état de la connexion
    QAbstractSocket::SocketState getState() const;

signals:

    void connectionError(const QString& strError);

    void connected();

public slots:

    //! Connecte au serveur
    void connectToHost(const QString& strHostIPAdress);

    //! reconnecte au serveur si déconnecté
    void reconnectToHost();

    //! Déconnecte du serveur
    void disconnectFromHost();

protected slots:

    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

protected:

    void doSendMessage();

    static const int iDTReconnection = 5000;

    QVariant		m_SendMessage;
    QTimer			m_RetryConnectionTimer;
    QTcpSocket*		m_TcpSocket;
    MaxPacketSize	m_iBlockSize;
    int				m_iPort;
    QString			m_strHostIPAdress;
    bool			m_bLastMessageNotSend;
};

#endif
