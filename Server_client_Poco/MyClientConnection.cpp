#include "StdAfx.h"

#include "CRC16.h"
#include  "zlib\include\zlib.h"
#include "xmlparser.h"
#include <iostream>
#include <stdlib.h>
#include <sstream> 
#include <ctime>
#include "MyClientConnection.h"

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Thread.h"

using Poco::Net::StreamSocket;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;


MyClientConnection::~MyClientConnection(void)
{
}

void MyClientConnection::run()
{
	SocketAddress sa2(this->server_address, 17003);	
    StreamSocket client;//(sa2);
	
	 
	

	 SocketAddress sa1(this->client_address, 50000+this->n);	
	 srv.bind(sa1);
	 srv.listen();

	
	 StreamSocket ss=srv.acceptConnection();
		   
		   
		   try
           {
             char buffer[65536];
			 
			 int size=0;
		
            int  n = ss.receiveBytes(buffer, sizeof(buffer)  );
		    
			
          for(;;)
	    	{   
			 char buffer2[1024]= { 0 };
			 time_t t = time(0);   // get time now
			 struct tm * now = localtime( & t );
			 client.connect(sa2);
			 composePlainMessage( buffer2,  size, nodeId+this->n);
			 int len = client.sendBytes(buffer2, size);	
			 std::cout << "Client   sent plain data  to server address  : "<<this->server_address<<" port "<< 17003 <<" with " << " NodID =  " << nodeId+this->n << " "<<now->tm_hour << "-"<<now->tm_min<< "-"<<now->tm_sec<<std::endl;
			 
			 client.close();
			
			 if (n>0 )
			  {
				 char*  request=_uncompress((byte*)&buffer, (WORD &)n);
				 decodeMessage( request, n);
				 composeMessage( buffer, n);
				 n = ss.sendBytes(buffer, n);
				 t = time(0);
				 now = localtime( & t );
				 std::cout << "Client   sent data  to server  from  client  : "<<this->client_address<<" port "<< ss.address().port()  << " with " << " I=" << connId << " C=" <<  msgCnt << " "<<now->tm_hour << "-"<<now->tm_min<< "-"<<now->tm_sec<<std::endl;
          		
			  }
			 ss=srv.acceptConnection();
             n = ss.receiveBytes(buffer, sizeof(buffer));
			
			

         }

		 
              
            } 
           catch (Poco::Exception& exc)
             {
                 std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
				
             }


}


char * MyClientConnection:: _uncompress ( byte * buf, WORD & len )
{
	uLongf destLen = * ( WORD * ) & buf [ UNZIP_LEN ];
	char*  request = new char [ destLen + 1 ];	
    int rez  = uncompress ( (Bytef*) request, &destLen, (Bytef*) &buf [ DATA_START ], ( uLongf  ) ( len - ADDITIONAL_BYTES ) );

	if (  rez != Z_OK )
	{
		delete [] request;
		return NULL;
	}
	request [ destLen ] = 0;
	return request;
}

bool MyClientConnection :: _compose ( byte * buf, WORD & len, char * request )
{	
	uLongf dLen  = strlen ( request );
	uLong buffLen = compressBound ( ( uLong ) dLen ) + ADDITIONAL_BYTES;

	int zlen = compress2 ( &buf [ DATA_START ], &buffLen, ( Bytef* )request, ( uLongf ) dLen, Z_BEST_COMPRESSION  );
	if ( zlen != Z_OK )
	{
		return false;
	}

	* ( WORD * ) & buf [ 0 ] = buffLen;
	* ( WORD * ) & buf [ 2 ] = dLen;
	* ( WORD * ) & buf [ buffLen + DATA_START ] = calcCRC16 ( buf, buffLen + ADDITIONAL_BYTES - CRC_LEN );

	len = buffLen + ADDITIONAL_BYTES;
	return true;
}

 

void MyClientConnection ::decodeMessage( char * buf, int  len)
{


	XMLNode xReply = XMLNode :: parseString ( strupr ( buf ),TAG_READ );
		if (xReply.isEmpty()) return ;
			connId = atoi ( xReply.getAttribute ( ATTR_CONN_ID ) );
			msgCnt = atoi ( xReply.getAttribute ( ATTR_MSG_CNT ) );
			
	return ;

};


void MyClientConnection ::composeMessage(char * buf, int  &len)
{

	char tempBuf [ 32 ] = { 0 };

	string template_xml(" ");
				
	XMLNode xRead = XMLNode ::parseString(template_xml.c_str(),TAG_READ);
	
	int num= xRead.nChildNode();  
	xRead.updateAttribute(  itoa ( connId, tempBuf, 10 ),ATTR_CONN_ID,ATTR_CONN_ID );
	xRead.updateAttribute ( itoa ( msgCnt, tempBuf, 10 ),ATTR_MSG_CNT ,ATTR_MSG_CNT);
	

	XMLNode xReadTN=xRead.getChildNodeWithAttribute("TNBB","alrm");
	
	xReadTN.updateAttribute(itoa ( rand()%10000 , tempBuf, 10 ),"alrm","alrm1");

	

	char * xmlReadRequest = xRead.createXMLString ( );

	_compose ( (byte*)buf, (WORD&)len, xmlReadRequest );

	
	free ( xmlReadRequest );



return;

	
};

void MyClientConnection ::composePlainMessage(char * buf, int  &len,int _node)
{

	
	char tempBuf [ 32 ] = { 0 };

	string template_xml("<TRBB=""1124"">");
		

	
	XMLNode xRead = XMLNode ::parseString(template_xml.c_str(),TAG_READ);// ( TAG_READ );

	
	int num= xRead.nChildNode();  
	xRead.updateAttribute(  itoa ( _node, tempBuf, 10 ),ATRR_NODE_ID,ATRR_NODE_ID );
	
	tempBuf[0]=0;

	XMLNode xReadTN=xRead.getChildNodeWithAttribute("TNBB","LAT");
		
	char * xmlReadRequest = xRead.createXMLString ( );

	strcpy( buf,  xmlReadRequest );
	len  = strlen ( xmlReadRequest );
	tempBuf[0]=0;
	
	free ( xmlReadRequest );



return;

	
};
