#ifndef DATATCPSERVER_H
#define DATATCPSERVER_H

#include <QTcpServer>

class DataTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DataTcpServer(QObject *parent = 0);
    void startServer(int portNumber=0);

signals:

public slots:
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // DATATCPSERVER_H
