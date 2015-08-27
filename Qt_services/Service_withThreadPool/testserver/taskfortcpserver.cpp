#include "taskfortcpserver.h"


TaskForTcpServer::TaskForTcpServer()
{

}


void TaskForTcpServer::run()
{
    if(!socketDescriptor) return;

        QTcpSocket socket;
        socket.setSocketDescriptor(socketDescriptor);
        //to do

        socket.write("From server: hello world");
        socket.flush();
        socket.waitForBytesWritten();
        socket.close();
}

