#include "CTcpServerTest.h"
#include "CTcpServer.h"

#include <QDataStream>
#include <QDateTime>
#include <QVariant>
#include <QDebug>


//-------------------------------------------------------------------------------------------------
QDataStream& operator << (QDataStream& out, const CUserMessage& userMessage)
{
    out << userMessage.name;
    return out;
}

//-------------------------------------------------------------------------------------------------
QDataStream& operator >> (QDataStream& in, CUserMessage& userMessage)
{
    in >> userMessage.name;
    return in;
}

//-------------------------------------------------------------------------------------------------
CTcpServerTest::CTcpServerTest()
{
    m_TcpServer = new CTcpServer();
    m_TcpServer->startListening(54321);

    connect(m_TcpServer, SIGNAL(connected(QString)),	this, SLOT(onConnected(QString)));
    connect(m_TcpServer, SIGNAL(disconnected(QString)), this, SLOT(onDisconnected(QString)));
    connect(m_TcpServer, SIGNAL(newMessage(QVariant)),	this, SLOT(onNewMessage(QVariant)));
}

//-------------------------------------------------------------------------------------------------
void CTcpServerTest::onConnected(const QString& strIP)
{
    qDebug() << "Connected : " << strIP;

    qRegisterMetaType<CUserMessage>("CUserMessage");
    qRegisterMetaTypeStreamOperators<CUserMessage>("CUserMessage");

    CUserMessage userMsg;

    sendMessage(QVariant::fromValue(userMsg));
}

//-------------------------------------------------------------------------------------------------
void CTcpServerTest::sendMessage(const QVariant& message)
{
    m_TcpServer->sendMessage(message);
}

//-------------------------------------------------------------------------------------------------
void CTcpServerTest::onDisconnected(const QString& strIP)
{
    qDebug() << "Disconnected : " << strIP;
}

//-------------------------------------------------------------------------------------------------
void CTcpServerTest::onNewMessage(const QVariant& message)
{
    qDebug() << message;
}
