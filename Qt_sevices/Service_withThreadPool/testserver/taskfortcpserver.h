#ifndef TASKFORTCPSERVER_H
#define TASKFORTCPSERVER_H

#include <QRunnable>
#include <QTcpSocket>

class TaskForTcpServer : public QRunnable
{

 public:

    //explicit TaskForTcpServer(QObject *parent = 0);
    TaskForTcpServer();


protected:
    void run();

public:
    qintptr socketDescriptor;

};

#endif // TASKFORTCPSERVER_H
