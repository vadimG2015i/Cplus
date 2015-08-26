#include "datatcpserver.h"
#include "taskfortcpserver.h"
#include "dataservice.h"
#include "DataLogger.h"



DataTcpServer::DataTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
}

void DataTcpServer::startServer(int portNumber)
{

    if(this->listen(QHostAddress::AnyIPv4, portNumber))
       {
           LOG4CXX_INFO(logger, "TCP Server started,listen port .." << portNumber);
       }
       else
       {
           LOG4CXX_DEBUG(logger, "TCP Server did not start!");
       }

}

void DataTcpServer::incomingConnection(qintptr handle)
{
    // 1. QTcpServer gets a new connection request from a client.
        // 2. It makes a task (runnable) here.
        // 3. Then, the server grabs one of the threads.
        // 4. The server throws the runnable to the thread.

        // Note: Rannable is a task not a thread
        TaskForTcpServer *task = new TaskForTcpServer();
        task->setAutoDelete(true);

        task->socketDescriptor = handle;

        pool->start(task);
        LOG4CXX_INFO(logger, "Thread pool started");

}
