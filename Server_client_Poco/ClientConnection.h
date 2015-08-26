#pragma once

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include  "Poco/Util/ServerApplication.h"
#include "Poco/Thread.h"
#include  "zlib\include\zlib.h"
#include "xmlparser.h"
#include <iostream>
#include <stdlib.h>

using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::StreamSocket;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;
using namespace std;

#define ADDITIONAL_BYTES 6
#define UNZIP_LEN 2
#define DATA_START 4
#define CRC_LEN 2

#define TAG_READ		"TR"
#define TAG_WRITE		"TW"
#define TAG_NAME		"TN"
#define ATTR_MSG_CNT	"C"
#define ATTR_CONN_ID	"I"

class ClientConnection :public TCPServerConnection
{

public:
	ClientConnection(const StreamSocket& s): TCPServerConnection(s)
     {
     }


	
public:
	~ClientConnection(void);

void run();
void composeMessage( char* buf, int  &len);
void decodeMessage( char * buf, int  len);
char * _uncompress ( byte * buf, WORD & len );
bool  _compose ( byte * buf, WORD & len, char * request );



int msgCnt, connId;
 StreamSocket  s;
      
       
   

};
