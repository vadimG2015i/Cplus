// TCPServerSocket.cpp: файл реализации
//

#include "stdafx.h"
#include "TcpServer.h"
#include "TCPServerSocket.h"
#include "TcpServerDlg.h"


// CTCPServerSocket

CTCPServerSocket::CTCPServerSocket()
{
}

CTCPServerSocket::~CTCPServerSocket()
{
}


// функции-члены CTCPServerSocket


void CTCPServerSocket::OnAccept(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	// AfxMessageBox(LPCTSTR("—оединение"),0,0);
    m_pDlg->OnAccept();
    CAsyncSocket::OnAccept(nErrorCode);
}






void CTCPServerSocket::OnReceive(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	//int socketNumber=this->m_hSocket;
	m_pDlg->m_pConnectSocket=this;
	m_pDlg->OnReceive();
	
	CAsyncSocket::OnReceive(nErrorCode);
}


void CTCPServerSocket::SetParentDlg(CTcpServerDlg *pDlg)
{
    m_pDlg=pDlg; //”казатель на диалоговое окно
}



