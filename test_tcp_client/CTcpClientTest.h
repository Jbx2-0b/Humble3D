#ifndef CTCPCLIENTTEST_H
#define CTCPCLIENTTEST_H

#include <QObject>
#include <QVariant>

#include "CTcpClient.h"


class CTcpClientTest : public QObject
{
    Q_OBJECT

public:

    CTcpClientTest();

public slots:

    void onNewMessage(const QVariant& component);

protected:

    CTcpClient* m_pTcpClient;
};

#endif // CTCPCLIENTTEST_H
