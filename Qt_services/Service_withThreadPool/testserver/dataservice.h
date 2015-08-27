#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QDir>
#include <QSettings>
#include "qtservice.h"
#include <QCoreApplication>
#include <QDebug>

#include <cstdlib>
#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

static   log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("myApp"));

class DataService: public QtService<QCoreApplication>
{
public:

    DataService(int argc, char **argv);
    ~DataService();

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);


};

#endif // DATASERVICE_H
