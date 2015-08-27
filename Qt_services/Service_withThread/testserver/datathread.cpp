#include "datathread.h"
#include "dataservice.h"


#include "datalogcxx.h"

DataThread::DataThread(qintptr ID, QObject *parent)
{
    this->socketDescriptor = ID;
}

void DataThread::run()
{
    // thread starts here
       LOG4CXX_DEBUG(logger, "  Thread started");

        socket = new QTcpSocket();

        // set the ID
        if(!socket->setSocketDescriptor(this->socketDescriptor))
        {
            // something's wrong, we just emit a signal
            emit error(socket->error());
            LOG4CXX_ERROR(logger, socket->errorString().toStdString());
            return;
        }

        // connect socket and signal
        // note - Qt::DirectConnection is used because it's multithreaded
        //        This makes the slot to be invoked immediately, when the signal is emitted.

        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

        // We'll have multiple clients, we want to know which is which
        LOG4CXX_DEBUG(logger, " Client connected  " << socketDescriptor );

        // make this thread a loop,
        // thread will stay alive so that signal/slot to function properly
        // not dropped out in the middle when thread dies

        exec();

}

void DataThread::readyRead()
{
    // get the information
        QByteArray Data = socket->readAll();

        // will write on server side window
       // LOG4CXX_DEBUG(logger, " Data in:  " << Data);

        //todo  some

        socket->write(Data);

}

void DataThread::disconnected()
{
       LOG4CXX_DEBUG(logger, " Disconnected "  << socketDescriptor ) ;


        socket->deleteLater();
        exit(0);

}
