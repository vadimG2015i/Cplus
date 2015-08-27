#include "dataservice.h"
#include "datatcpserver.h"

DataService::DataService(int argc, char **argv):QtService<QCoreApplication>(argc,argv,"DataService")
{

    try
    {
        setServiceDescription("DataService first release");
        setServiceFlags(QtService::CanBeSuspended);

    }
    catch(...)
    {
        qCritical()<< "Unknown error!";

    }
}

DataService:: ~DataService()
{

    try
    {
       //To do

    }
    catch(...)
    {
        qCritical()<< "Unknown error destructor!";

    }
}


void DataService::start()
{

    try
    {
       //To do
          QCoreApplication *app = application();

           LOG4CXX_INFO( logger, app->applicationFilePath().toStdString());
           DataTcpServer *server=new DataTcpServer();
           server->startServer(17002);
        // qDebug()<<"Service started";
         //qDebug()<<app->applicationFilePath();
         //LOG4CXX_INFO( logger, "Hello World!" );
           LOG4CXX_INFO( logger, "Service started" );

    }
    catch(...)
    {
        qCritical()<< "Unknown error !";

    }

}

void DataService::stop()
{

    try
    {
       //To do

        LOG4CXX_INFO( logger, "Service stopped");
    }
    catch(...)
    {
        qCritical()<< "Unknown error !";

    }
}

void DataService::pause()
{

    try
    {
       //To do
       qDebug()<<"Service paused";

    }
    catch(...)
    {
        qCritical()<< "Unknown error !";

    }

}

void DataService::resume()
{


    try
    {
       //To do
        qDebug()<<"Service resumed";

    }
    catch(...)
    {
        qCritical()<< "Unknown error !";

    }
}

void DataService::processCommand(int code)
{

    try
    {
       //To do

         qDebug()<<"Service processed command";
    }
    catch(...)
    {
        qCritical()<< "Unknown error !";

    }

};





