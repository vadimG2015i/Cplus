#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class DataThread : public QThread
{
    Q_OBJECT
public:
    explicit DataThread(qintptr ID, QObject *parent = 0);
    void run();

signals:
     void error(QTcpSocket::SocketError socketerror);

public slots:
     void readyRead();
     void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
};

#endif // DATATHREAD_H
