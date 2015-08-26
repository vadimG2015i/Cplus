#pragma once

// конечный объект команды CTCPServerSocket
//#include "TcpServerDlg.h"
class CTcpServerDlg;
class CTCPServerSocket : public CAsyncSocket
{
public:
	CTCPServerSocket();
	CTcpServerDlg  *m_pDlg;
	virtual ~CTCPServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void SetParentDlg (CTcpServerDlg *pDlg);
};


