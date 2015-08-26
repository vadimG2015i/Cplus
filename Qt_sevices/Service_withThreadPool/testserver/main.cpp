#include <QCoreApplication>
#include "dataservice.h"

#include <cstdlib>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include "DataLogger.h"

int main(int argc, char *argv[])
{
      QString path=QFileInfo(argv[0]).dir().path();
      QString initFileName = path + "/log4cxx.properties";

      log4cxx::BasicConfigurator::configure();
      log4cxx::File propertyFile ( initFileName.toStdString());
      log4cxx::PropertyConfigurator::configure( propertyFile );
      LOG4CXX_INFO(logger,"Reading  log4cxx.properties.");

      // Write to the log
       LOG4CXX_INFO(logger,"Entering application.");
       LOG4CXX_DEBUG(logger, "This is a debug message");

      DataService service(argc, argv);

      return service.exec();
      LOG4CXX_INFO(logger,"Exiting application.");

}
