#ifndef CTCPSERVERTEST_H
#define CTCPSERVERTEST_H

#include <QObject>
#include <QVariant>

class CTcpServer;

//-------------------------------------------------------------------------------------------------
class CUserMessage
{
public:

    CUserMessage() {}

protected:

    QString name;

    friend QDataStream& operator << (QDataStream& out,	const CUserMessage& userMessage);
    friend QDataStream& operator >> (QDataStream& in,	CUserMessage& userMessage);

};
Q_DECLARE_METATYPE(CUserMessage)

QDataStream& operator << (QDataStream& out,	const CUserMessage& userMessage);
QDataStream& operator >> (QDataStream& in,	CUserMessage& userMessage);

//-------------------------------------------------------------------------------------------------
class CTcpServerTest : public QObject
{
    Q_OBJECT

public:

    CTcpServerTest();

    void sendMessage(const QVariant& message);

public slots:

    void onConnected(const QString& strIP);
    void onDisconnected(const QString& strIP);
    void onNewMessage(const QVariant& strMessage);

private:

    CTcpServer* m_TcpServer;
};


#endif // CTCPSERVERTEST_H
