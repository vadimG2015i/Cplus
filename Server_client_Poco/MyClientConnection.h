#pragma once
#include "Poco/Runnable.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"

using Poco::Runnable;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;

using namespace std;

#define ADDITIONAL_BYTES 6
#define UNZIP_LEN 2
#define DATA_START 4
#define CRC_LEN 2

#define TAG_READ		"TRBB"
#define TAG_WRITE		"TWBB"
#define TAG_NAME		"TNBB"
#define ATTR_MSG_CNT	"C"
#define ATTR_CONN_ID	"I"
#define ATRR_NODE_ID     "NDBB"


class MyClientConnection :	public Runnable
{
public:
	MyClientConnection(int repetitions=-1,string host_local="",string host_remote=""):n(repetitions),client_address(host_local),server_address(host_remote)
	{
	msgCnt=0; connId=0;nodeId=2005;
	srv=ServerSocket();
	   
	}

public:
	~MyClientConnection(void);

	void run();

	void composeMessage( char* buf, int  &len);
	void composePlainMessage( char* buf, int  &len,int _node);
    void decodeMessage( char * buf, int  len);
    char * _uncompress ( byte * buf, WORD & len );
    bool  _compose ( byte * buf, WORD & len, char * request );
	int msgCnt, connId,nodeId;
	ServerSocket srv;
	SocketAddress sa;
	StreamSocket ss;
	
private:
	int n;
	string client_address;
	string server_address;
};
