#include "CTcpClientTest.h"

#include <QDateTime>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>

//-----------------------------------------------------------------------------------------
CTcpClientTest::CTcpClientTest()
{
    m_pTcpClient = new CTcpClient(54321);
    m_pTcpClient->connectToHost("127.0.0.1");

    connect(m_pTcpClient, SIGNAL(newMessage(QVariant)), this, SLOT(onNewMessage(QVariant)));


    //m_TcpClient->sendMessage(QVariant::fromValue(component));
}

//-----------------------------------------------------------------------------------------
void CTcpClientTest::onNewMessage(const QVariant& message)
{
    qDebug() << message;

    for (int i = 0; i < 100; ++i)
    {
        m_pTcpClient->sendMessage(QVariant::fromValue(QString("Bliblo")));
    }
}
