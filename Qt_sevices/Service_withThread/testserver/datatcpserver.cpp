#include "datatcpserver.h"
#include "datathread.h"
#include "dataservice.h"
#include "datalogcxx.h"


DataTcpServer::DataTcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

void DataTcpServer::startServer(int portNumber)
{

    //int port = 17002;

        if(!this->listen(QHostAddress::AnyIPv4, portNumber))
     //   if(!this->listen(QHostAddress::Any, portNumber))
        {
           LOG4CXX_DEBUG(logger,  "Could not start server");
        }
        else
        {
          LOG4CXX_INFO( logger, "DataTcpServer started on port " << portNumber);
          LOG4CXX_DEBUG(logger, "Listening to port " << portNumber << "...");
        }

}

void DataTcpServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
        LOG4CXX_DEBUG(logger,  " Connecting..." << socketDescriptor);

        // Every new connection will be run in a newly created thread
        DataThread *thread = new DataThread(socketDescriptor, this);

        // connect signal/slot
        // once a thread is not needed, it will be beleted later
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        thread->start();

}
