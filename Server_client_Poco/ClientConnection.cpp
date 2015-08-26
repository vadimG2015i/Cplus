#include "stdafx.h"
#include "ClientConnection.h"
#include "CRC16.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/XMLConfiguration.h"
#include <sstream> 
#include <ctime>


#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/Exception.h"
#include <iostream>


using Poco::XML::DOMParser;
using Poco::XML::InputSource;
using Poco::XML::Document;
using Poco::XML::NodeIterator;
using Poco::XML::NodeFilter;
using Poco::XML::Node;
using Poco::XML::AutoPtr;
using Poco::Exception;


using namespace Poco::Util;
using namespace std;

ClientConnection::~ClientConnection(void)
{

}


void ClientConnection ::run()
       {
		   StreamSocket& ss = socket();
		   
		  try
           {
            char buffer[65536];
		
            int  n = ss.receiveBytes(buffer, sizeof(buffer));
		    char*  request=_uncompress((byte*)&buffer, (WORD &)n);
		    decodeMessage( request, n);
			composeMessage( buffer, n);
			
            while (n > 0) 
			 {
              //ss.sendBytes(buffer, n);
			  //Sleep(5000);
              n = ss.sendBytes(buffer, n);
			  time_t t = time(0);   // get time now
              struct tm * now = localtime( & t );
          
			  std::cout << "Client "<<" port "<<this->socket().address().port()<<"  sent data  with " << " I=" << connId << " C=" <<  msgCnt << " "<<now->tm_hour << "-"<<now->tm_min<< "-"<<now->tm_sec<<std::endl;
		
			  //this->socket().address().port();string out(buffer,n);
		  	//std::cout << "Begin   "<< out<<"   End " << std::endl;
			  Sleep(15000);
              n = ss.receiveBytes(buffer, sizeof(buffer));
			  decodeMessage( request, n);
			  composeMessage( buffer, n);

             }

		   
              
            } 
           catch (Poco::Exception& exc)
             {
                 std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
             }
       
   };




char * ClientConnection:: _uncompress ( byte * buf, WORD & len )
{
	uLongf destLen = * ( WORD * ) & buf [ UNZIP_LEN ];
	char*  request = new char [ destLen + 1 ];	
    int rez  = uncompress ( (Bytef*) request, &destLen, (Bytef*) &buf [ DATA_START ], ( uLongf  ) ( len - ADDITIONAL_BYTES ) );

	if (  rez != Z_OK )
	{
		//gLog.logStr ( Error, "XmlDriver :: error in uncompress %i", rez );
		delete [] request;
		return NULL;
	}
	request [ destLen ] = 0;
	return request;
}

bool ClientConnection :: _compose ( byte * buf, WORD & len, char * request )
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

 

void ClientConnection ::decodeMessage( char * buf, int  len)
{


	XMLNode xReply = XMLNode :: parseString ( strupr ( buf ),TAG_READ );
		if (xReply.isEmpty()) return ;
			connId = atoi ( xReply.getAttribute ( ATTR_CONN_ID ) );
			msgCnt = atoi ( xReply.getAttribute ( ATTR_MSG_CNT ) );
			
	return ;

};


void ClientConnection ::composeMessage(char * buf, int  &len)
{

	string fileName = "d://test3.xml";

	char tempBuf [ 32 ] = { 0 };

	XMLNode xRead= XMLNode::parseFile(fileName.c_str(),TAG_READ );

	int num= xRead.nChildNode();  
	xRead.updateAttribute(  itoa ( connId, tempBuf, 10 ),ATTR_CONN_ID,ATTR_CONN_ID );
	xRead.updateAttribute ( itoa ( msgCnt, tempBuf, 10 ),ATTR_MSG_CNT ,ATTR_MSG_CNT);
	

	XMLNode xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.alrm.w0");
	int port_now =this->socket().address().port();//.this->Poco::Net::SocketAddress::port();
	
	xReadTN.updateAttribute(itoa ( rand()%10000 , tempBuf, 10 ),"GEN1.alrm.w0","GEN1.alrm.w0");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ua");
	xReadTN.updateAttribute(itoa ( rand()%21 +220, tempBuf, 10 ),"GEN1.Ua","GEN1.Ua");
	


	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ua");
	xReadTN.updateAttribute(itoa ( rand()%21 +210, tempBuf, 10 ),"GEN1.Ua","GEN1.Ua");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ub");
	xReadTN.updateAttribute(itoa ( rand()%21 +210, tempBuf, 10 ),"GEN1.Ub","GEN1.Ub");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Uc");
	xReadTN.updateAttribute(itoa ( rand()%21 +210, tempBuf, 10 ),"GEN1.Uc","GEN1.Uc");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ia");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Ia","GEN1.Ia");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ib");
	xReadTN.updateAttribute(itoa  (rand()%149 +50, tempBuf, 10 ),"GEN1.Ib","GEN1.Ib");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Ic");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Ic","GEN1.Ic");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.P");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.P","GEN1.P");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Q");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Q","GEN1.Q");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.S");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.S","GEN1.S");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Pa");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Pa","GEN1.Pa");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Pb");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Pb","GEN1.Pb");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Pc");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Pc","GEN1.Pc");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Qa");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Qa","GEN1.Qa");

	xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Qb");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Qb","GEN1.Qb");

    xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Qc");
	xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Qc","GEN1.Qc");

	 xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Sa");
	 xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Sa","GEN1.Sa");

	 xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Sb");
	 xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Sb","GEN1.Sb");

	 xReadTN=xRead.getChildNodeWithAttribute("TN","GEN1.Sc");
	 xReadTN.updateAttribute(itoa ( rand()%149 +50, tempBuf, 10 ),"GEN1.Sc","GEN1.Sc");


	char * xmlReadRequest = xRead.createXMLString ( );

	_compose ( (byte*)buf, (WORD&)len, xmlReadRequest );

	
	free ( xmlReadRequest );



return;

	//+		request	0x03f10470 "<TR I="4" C="0"><TN ALARM.ENABLED="1"/><TN ALARM.QUAL="1"/><TN ALARM.RED="0"/><TN ALARM.RED_LINE="0"/><TN ALARM.UPS1.ALARM.CONNECTION="0"/><TN ALARM.YELLOW="0"/><TN ALARM.YELLOW_LINE="0"/><TN ATI1.ALARM.KEY_MODE="0"/><TN ATI1.ALARM.PHASE_A="0"/><TN ATI1.ALARM.PHASE_B="0"/><TN ATI1.ALARM.PHASE_C="0"/><TN ATI1.ALARM.RED="0"/><TN ATI1.ALARM.SWITCH="0"/><TN ATI1.ALARM.YELLOW="0"/><TN ATI1.ALRM.W0="0"/><TN ATI1.CONNSRC="1"/><TN ATI1.ENABLED="1"/><TN ATI1.GEN.FREQ="0.000"/><TN ATI1.GEN.UA="0.000"/><TN ATI1.GEN.UB="0.000"/><TN ATI1.GEN.UC="0.000"/><TN ATI1.LOAD.FACTOR="0.000"/><TN ATI1.LOAD.IA="0.000"/><TN ATI1.LOAD.IB="0.000"/><TN ATI1.LOAD.IC="0.000"/><TN ATI1.LOAD.IN="0.000"/><TN ATI1.LOAD.P="0.000"/><TN ATI1.LOAD.Q="0.000"/><TN ATI1.LOAD.S="0.000"/><TN ATI1.MAIN.FREQ="49.990"/><TN ATI1.MAIN.UA="221.900"/><TN ATI1.MAIN.UB="223.400"/><TN ATI1.MAIN.UC="222.000"/><TN ATI1.MODE="0"/><TN ATI1.QUAL="1"/><TN ATI1.SWITCH="16"/><TN ATI1.SWITCH1="16"/><TN ATI1.SWITCHSTATE="0"/><TN ATI2.ALARM.KEY_MODE="1"/><TN"	char *
	//+		request	0x03f17f30 "<TR I="4" C="1"><TN ATI1.MAIN.UA="221.300"/><TN ATI2.MAIN.UC="226.100"/><TN DMK1.IA="181.100"/><TN DMK1.IB="176.200"/><TN DMK1.IC="165.100"/><TN DMK1.IN="13.200"/><TN DMK1.P="115.500"/><TN DMK1.Q="13.000"/><TN DMK1.S="116.200"/><TN DMK1.UB="222.900"/><TN DMK1.WP.LO="23.750"/><TN DMK1.WQ.LO="-161.980"/><TN DMK2.COS="93"/><TN DMK2.COSA="93"/><TN DMK2.COSB="94"/><TN DMK2.IA="87.000"/><TN DMK2.IC="104.000"/><TN DMK2.IEQ="102.000"/><TN DMK2.P="65800"/><TN DMK2.PA="18600"/><TN DMK2.PB="24700"/><TN DMK2.PC="22700"/><TN DMK2.Q="-2147458448"/><TN DMK2.QA="-2147476518"/><TN DMK2.QB="-2147474798"/><TN DMK2.QC="-2147475718"/><TN DMK2.S="70500"/><TN DMK2.SA="19900"/><TN DMK2.SB="26200"/><TN DMK2.SC="24100"/><TN DMK2.WAIMP="119704.695"/><TN DMK2.WREXP="45668.750"/><TN GEN1.COOLTEMP="49.719"/><TN UPS1.IN.LINEVOLT="221"/><TN UPS1.IN.MAXLINEVOL="223"/><TN UPS1.IN.MINLINEVOLT="220"/><TN UPS1.OUT.CURRENT="113"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE1="105.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE2="102."	char *
	//+		request	0x04695bc0 "<TR I="4" C="2"><TN ATI1.MAIN.UA="221.900"/><TN ATI1.MAIN.UC="222.600"/><TN ATI2.GEN.FREQ="49.990"/><TN DMK1.IA="191.200"/><TN DMK1.IB="184.100"/><TN DMK1.IC="174.000"/><TN DMK1.P="120.800"/><TN DMK1.Q="16.000"/><TN DMK1.S="121.900"/><TN DMK1.WP.LO="25.180"/><TN DMK1.WQ.LO="-160.540"/><TN DMK2.COS="94"/><TN DMK2.COSA="91"/><TN DMK2.COSB="93"/><TN DMK2.IA="86.800"/><TN DMK2.IEQ="101.000"/><TN DMK2.P="66200"/><TN DMK2.PA="18400"/><TN DMK2.PB="24600"/><TN DMK2.PC="22800"/><TN DMK2.Q="-2147459848"/><TN DMK2.QA="-2147475718"/><TN DMK2.QB="-2147474598"/><TN DMK2.QC="-2147475728"/><TN DMK2.S="70400"/><TN DMK2.SA="20000"/><TN DMK2.SC="24100"/><TN DMK2.WAIMP="119704.773"/><TN DMK2.WREXP="45668.777"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE3="103.000"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE2="223"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE3="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE2="223"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE3="222"/><TN UPS1.UPSPHASEOUTPUTCURRENT.OUT1.PHASE1="86.000""	char *
	//+		request	0x03f18ec0 "<TR I="4" C="3"><TN ATI1.MAIN.UA="221.100"/><TN ATI1.MAIN.UB="222.700"/><TN ATI1.MAIN.UC="221.800"/><TN ATI2.GEN.FREQ="50.000"/><TN ATI2.MAIN.UA="223.000"/><TN ATI2.MAIN.UB="225.200"/><TN DMK1.IA="180.500"/><TN DMK1.IB="178.500"/><TN DMK1.IC="167.400"/><TN DMK1.IN="11.600"/><TN DMK1.P="115.700"/><TN DMK1.Q="13.700"/><TN DMK1.S="116.500"/><TN DMK1.UA="220.700"/><TN DMK1.WP.LO="26.640"/><TN DMK1.WQ.LO="-159.070"/><TN DMK2.COS="94"/><TN DMK2.COSA="92"/><TN DMK2.COSB="94"/><TN DMK2.COSC="94"/><TN DMK2.IA="86.600"/><TN DMK2.IEQ="102.000"/><TN DMK2.PA="18500"/><TN DMK2.PB="24900"/><TN DMK2.PC="22800"/><TN DMK2.Q="-2147459648"/><TN DMK2.QA="-2147476148"/><TN DMK2.QB="-2147474968"/><TN DMK2.QC="-2147475738"/><TN DMK2.S="70400"/><TN DMK2.SB="26300"/><TN DMK2.SC="24100"/><TN DMK2.WAIMP="119704.867"/><TN DMK2.WREXP="45668.809"/><TN UPS1.OUT.CURRENT="112"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE1="106.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE2="102.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE3"	char *
	//+		request	0x0474d5d0 "<TR I="2" C="0"><TN ALARM.ENABLED="1"/><TN ALARM.QUAL="1"/><TN ALARM.RED="0"/><TN ALARM.RED_LINE="0"/><TN ALARM.UPS1.ALARM.CONNECTION="0"/><TN ALARM.YELLOW="0"/><TN ALARM.YELLOW_LINE="0"/><TN ATI1.ALARM.KEY_MODE="0"/><TN ATI1.ALARM.PHASE_A="0"/><TN ATI1.ALARM.PHASE_B="0"/><TN ATI1.ALARM.PHASE_C="0"/><TN ATI1.ALARM.RED="0"/><TN ATI1.ALARM.SWITCH="0"/><TN ATI1.ALARM.YELLOW="0"/><TN ATI1.ALRM.W0="0"/><TN ATI1.CONNSRC="1"/><TN ATI1.ENABLED="1"/><TN ATI1.GEN.FREQ="0.000"/><TN ATI1.GEN.UA="0.000"/><TN ATI1.GEN.UB="0.000"/><TN ATI1.GEN.UC="0.000"/><TN ATI1.LOAD.FACTOR="0.000"/><TN ATI1.LOAD.IA="0.000"/><TN ATI1.LOAD.IB="0.000"/><TN ATI1.LOAD.IC="0.000"/><TN ATI1.LOAD.IN="0.000"/><TN ATI1.LOAD.P="0.000"/><TN ATI1.LOAD.Q="0.000"/><TN ATI1.LOAD.S="0.000"/><TN ATI1.MAIN.FREQ="49.990"/><TN ATI1.MAIN.UA="221.100"/><TN ATI1.MAIN.UB="223.200"/><TN ATI1.MAIN.UC="221.800"/><TN ATI1.MODE="0"/><TN ATI1.QUAL="1"/><TN ATI1.SWITCH="16"/><TN ATI1.SWITCH1="16"/><TN ATI1.SWITCHSTATE="0"/><TN ATI2.ALARM.KEY_MODE="1"/><TN"	char *
	//+		request	0x046a5d70 "<TR I="2" C="1"><TN ATI1.MAIN.UB="222.500"/><TN ATI2.MAIN.UA="223.000"/><TN DMK1.IA="192.200"/><TN DMK1.WP.LO="28.160"/><TN DMK1.WQ.LO="-157.540"/><TN DMK1.WS.LO="76.390"/><TN DMK2.COSB="94"/><TN DMK2.COSC="94"/><TN DMK2.P="65900"/><TN DMK2.PA="18600"/><TN DMK2.PC="23000"/><TN DMK2.Q="-2147458848"/><TN DMK2.QA="-2147476118"/><TN DMK2.QB="-2147474878"/><TN DMK2.QC="-2147475998"/><TN DMK2.S="70500"/><TN DMK2.SA="20100"/><TN DMK2.SB="26200"/><TN DMK2.SC="24200"/><TN DMK2.WAIMP="119704.945"/><TN DMK2.WREXP="45668.840"/><TN UPS1.IN.LINEVOLT="221"/><TN UPS1.OUT.CURRENT="113"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE3="220"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE1="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE3="220"/><TN UPS1.UPSPHASEOUTPUTCURRENT.OUT1.PHASE1="85.000"/><TN UPS1.UPSPHASEOUTPUTCURRENT.OUT1.PHASE3="103.000"/><TN UPS1.UPSPHASEOUTPUTLOAD.OUT1.PHASE1="19910"/><TN UPS1.UPSPHASEOUTPUTLOAD.OUT1.PHASE2="26260"/><TN UPS1.UPSPHASEOUTPUTLOAD.OUT1.PHASE3="24190"/></TR>"	char *
	//+		request	0x046a2da0 "<TR I="2" C="2"><TN ATI1.MAIN.UA="221.600"/><TN ATI2.MAIN.UA="223.900"/><TN DMK1.IA="190.700"/><TN DMK2.COSA="91"/><TN DMK2.COSB="93"/><TN DMK2.IA="86.500"/><TN DMK2.P="65800"/><TN DMK2.PA="18300"/><TN DMK2.PB="24600"/><TN DMK2.PC="22700"/><TN DMK2.Q="-2147458748"/><TN DMK2.QA="-2147475658"/><TN DMK2.QB="-2147474368"/><TN DMK2.QC="-2147475668"/><TN DMK2.S="70400"/><TN DMK2.SA="20000"/><TN DMK2.SB="26300"/><TN DMK2.SC="24100"/><TN DMK2.WAIMP="119704.984"/><TN DMK2.WREXP="45668.848"/><TN UPS1.OUT.LOAD="60"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE1="106.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE2="102.000"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE1="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE2="223"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE3="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE1="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE2="223"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE3="221"/><TN UPS1.UPSPHASEOUTPUTCURRENT.OUT1.PHASE1="86.000"/><TN UPS1.UPSPHASEOUTPUTCURRENT"	char *
	//+		request	0x0475b030 "<TR I="5" C="0"><TN ALARM.ENABLED="1"/><TN ALARM.QUAL="1"/><TN ALARM.RED="0"/><TN ALARM.RED_LINE="0"/><TN ALARM.UPS1.ALARM.CONNECTION="0"/><TN ALARM.YELLOW="0"/><TN ALARM.YELLOW_LINE="0"/><TN ATI1.ALARM.KEY_MODE="0"/><TN ATI1.ALARM.PHASE_A="0"/><TN ATI1.ALARM.PHASE_B="0"/><TN ATI1.ALARM.PHASE_C="0"/><TN ATI1.ALARM.RED="0"/><TN ATI1.ALARM.SWITCH="0"/><TN ATI1.ALARM.YELLOW="0"/><TN ATI1.ALRM.W0="0"/><TN ATI1.CONNSRC="1"/><TN ATI1.ENABLED="1"/><TN ATI1.GEN.FREQ="0.000"/><TN ATI1.GEN.UA="0.000"/><TN ATI1.GEN.UB="0.000"/><TN ATI1.GEN.UC="0.000"/><TN ATI1.LOAD.FACTOR="0.000"/><TN ATI1.LOAD.IA="0.000"/><TN ATI1.LOAD.IB="0.000"/><TN ATI1.LOAD.IC="0.000"/><TN ATI1.LOAD.IN="0.000"/><TN ATI1.LOAD.P="0.000"/><TN ATI1.LOAD.Q="0.000"/><TN ATI1.LOAD.S="0.000"/><TN ATI1.MAIN.FREQ="49.990"/><TN ATI1.MAIN.UA="221.600"/><TN ATI1.MAIN.UB="223.100"/><TN ATI1.MAIN.UC="221.800"/><TN ATI1.MODE="0"/><TN ATI1.QUAL="1"/><TN ATI1.SWITCH="16"/><TN ATI1.SWITCH1="16"/><TN ATI1.SWITCHSTATE="0"/><TN ATI2.ALARM.KEY_MODE="1"/><TN"	char *
	//+		request	0x04766f50 "<TR I="5" C="1"><TN ATI2.GEN.FREQ="50.000"/><TN ATI2.MAIN.UA="223.100"/><TN DMK1.IA="166.500"/><TN DMK1.IB="163.800"/><TN DMK1.IC="154.000"/><TN DMK1.P="107.500"/><TN DMK1.Q="9.000"/><TN DMK1.S="107.800"/><TN DMK1.THD.IB="7.400"/><TN DMK1.UA="221.800"/><TN DMK1.UB="223.900"/><TN DMK2.COSB="93"/><TN DMK2.IA="86.900"/><TN DMK2.IB="115.000"/><TN DMK2.IC="105.000"/><TN DMK2.IEQ="102.000"/><TN DMK2.P="66500"/><TN DMK2.PA="18500"/><TN DMK2.PC="22900"/><TN DMK2.Q="-2147458948"/><TN DMK2.QA="-2147476208"/><TN DMK2.QB="-2147474628"/><TN DMK2.QC="-2147475648"/><TN DMK2.S="70900"/><TN DMK2.SA="20000"/><TN DMK2.SB="26400"/><TN DMK2.SC="24200"/><TN DMK2.WAIMP="119705.070"/><TN DMK2.WREXP="45668.879"/><TN GEN1.COOLTEMP="48.625"/><TN UPS1.IN.MAXLINEVOL="223"/><TN UPS1.OUT.CURRENT="113"/><TN UPS1.OUT.LOAD="61"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE1="105.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE2="102.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE3="103.000"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.P"	char *
	//+		request	0x046a4168 "<TR I="5" C="2"><TN ATI2.GEN.FREQ="49.990"/><TN DMK1.IA="173.600"/><TN DMK1.IB="168.800"/><TN DMK1.IC="159.000"/><TN DMK1.P="111.200"/><TN DMK1.S="111.700"/><TN DMK1.THD.IC="7.300"/><TN DMK1.WP.LO="31.060"/><TN DMK1.WQ.LO="-154.620"/><TN DMK2.COSA="93"/><TN DMK2.COSB="94"/><TN DMK2.IA="86.800"/><TN DMK2.IB="113.000"/><TN DMK2.IC="104.000"/><TN DMK2.IEQ="101.000"/><TN DMK2.P="65900"/><TN DMK2.PA="18600"/><TN DMK2.PB="24700"/><TN DMK2.PC="22600"/><TN DMK2.Q="-2147459348"/><TN DMK2.QA="-2147476478"/><TN DMK2.QB="-2147474898"/><TN DMK2.QC="-2147475488"/><TN DMK2.S="70300"/><TN DMK2.SA="19900"/><TN DMK2.SB="26200"/><TN DMK2.SC="24000"/><TN DMK2.WAIMP="119705.109"/><TN DMK2.WREXP="45668.898"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE1="106.000"/><TN UPS1.UPSPHASEINPUTCURRENT.INPUT1.PHASE3="104.000"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE2="223"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT1.PHASE3="221"/><TN UPS1.UPSPHASEINPUTVOLTAGE.INPUT2.PHASE3="222"/><TN UPS1.UPSPHASEOUTPUTCURRENT.OUT1.PHASE1="85.000""	char *

};



