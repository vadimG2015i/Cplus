// dataclient3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ClientConnection.h"
#include "MyClientConnection.h"
#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include<cstdlib>






using namespace Poco::Net;
using namespace Poco::Util;
using Poco::Runnable;
using Poco::Thread;
using Poco::Stopwatch;
using Poco::ThreadPool;

using namespace std;

void  CopyCfgFiles(string path,int numbers)
{
	string fileName = path;
	fileName.append("/test.xnod");
	
	
	

	char tempBuf [ 32 ] = { 0 };
	//XMLNode xRead = XMLNode ::parseString(strupr (fileBuf),"NODE",&xe);// ( TAG_READ )
	XMLNode xReadMost= XMLNode::parseFile(fileName.c_str(),"MOST");
	int num=xReadMost.nChildNode();  
	XMLNode xReadNodeConfig =xReadMost.getChildNode("RealNodeConfig");
    XMLNode xReadNode =xReadNodeConfig.getChildNode("Node");
	XMLNode xReadNodeLan =xReadNode.getChildNode("LAN");
	
	for(int  i=1;i<=numbers;i++)
	{
	string fileNameOut = path;
	fileNameOut.append("/test");
	string idName = "test_";
	idName.append( itoa (1001+i, tempBuf, 10 ));

	xReadNode.updateAttribute(  itoa ( 2005+i, tempBuf, 10 ),"id","id" );
	xReadNode.updateAttribute ( idName.c_str(),"name" ,"name");	
	xReadNodeLan.updateAttribute("127.0.0.1","ip","ip");
	xReadNodeLan.updateAttribute(itoa ( 50001+i, tempBuf, 10 ),"port","port");

	char * xmlReadRequest = xReadMost.createXMLString ( );
	//char * xmlReadRequest1 = xReadNodeLan.createXMLString ( );

	fileNameOut.append(itoa( i, tempBuf, 10 ));
	fileNameOut.append(".xnod");
    xReadMost.writeToFile(fileNameOut.c_str(),NULL,1);
	free ( xmlReadRequest );
	

	}

	return;

}


class DataServerApp : public ServerApplication
{
protected:
  int main(const std::vector<std::string>& args)
  {
  

	  if ( args.empty())
		{
			return 0;
		}

  try
  {
		 std::vector<Thread*> threads;
		 std::vector<MyClientConnection *> conns;
	     Stopwatch sw;
		 sw.start();
		 std::vector<std::string>::const_iterator it = args.begin();
		 string host(*it++);
		 string host2(*it);
		

	    
			for (int i = 1; i <=100; i++)
			{
			
			Thread* pt = new Thread();
			poco_check_ptr(pt);
			threads.push_back(pt);

			MyClientConnection *srv =new  MyClientConnection(i,host,host2);
			poco_check_ptr(srv);
			conns.push_back(srv);
			threads.back()->start(*srv);
							
			cout << endl << "Server started " <<50000+i<< endl;
			
			}
//
			waitForTerminationRequest();  // wait for CTRL-C or kill
			 //system("PAUSE");
			//Poco::ThreadPool::defaultPool().stopAll();
			//Poco::ThreadPool::defaultPool().stopAll();
			//Poco::ThreadPool::defaultPool().stopAll();
			std::vector<MyClientConnection *>::iterator itc = conns.begin();
		//	for(; itc != conns.end(); ++itc)
		//	{
				
			//	(*itc)->srv.close();
				//delete (*itc);
				
		//	}

		/*	std::vector< Thread* >::iterator _thread = threads.begin();
			for(; _thread != threads.end(); ++_thread)
			{
				
				(*_thread)->join();
				//delete (*itc);
				
			}*/
			Poco::ThreadPool::defaultPool().stopAll();

		  /*  std::vector<Thread*>::iterator it = threads.begin();
			for(; it != threads.end(); ++it)
			{
				
				(*it)->join();
				delete *it;
			}*/
		sw.stop();
	   } 
           catch (Poco::Exception& exc)
             {
                 std::cerr << "Application : " << exc.displayText() << std::endl;
				 //ss = srv.acceptConnection();
             }
	

    return Application::EXIT_OK;
  }
};


int _tmain(int argc, _TCHAR* argv[])
{
	//CopyCfgFiles("D:/projects_r/DataServer_POCO/Debug/Config",100);
	DataServerApp app;
    return app.run(argc, argv); 
	
}

