#pragma once

#include "Poco/Net/ServerSocket.h"



using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;

class DataServerSocket :	public ServerSocket
{
public:
	DataServerSocket()
	{
		 n=0;
	}


public:
	DataServerSocket(const SocketAddress& address,int k=-1):ServerSocket(address),n(k)
	{
	}
	


public:
	~DataServerSocket();

	int n;
};
