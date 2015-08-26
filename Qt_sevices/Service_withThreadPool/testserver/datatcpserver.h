#ifndef DATATCPSERVER_H
#define DATATCPSERVER_H


#include <QTcpServer>
#include <QThreadPool>


class DataTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit DataTcpServer(QObject *parent = 0);
    void startServer(int portNumber );
protected:
    void incomingConnection( qintptr handle );
signals:

public slots:
private:
    QThreadPool *pool;
};

#endif // DATATCPSERVER_H
