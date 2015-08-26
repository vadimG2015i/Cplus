// TCPClientSocket.cpp: файл реализации
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientSocket.h"
#include "TCPClientDlg.h"


// CTCPClientSocket

CTCPClientSocket::CTCPClientSocket()
{
}

CTCPClientSocket::~CTCPClientSocket()
{
}




// функции-члены CTCPClientSocket


void CTCPClientSocket::SetParentDlg(CTCPClientDlg *pDlg)
{
    m_pDlg=pDlg; //Задаем указатель на диалоговое окно
}

void CTCPClientSocket::OnReceive(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	m_pDlg->OnReceive();

	CAsyncSocket::OnReceive(nErrorCode);
}
