// TCPServerSocket.cpp: ���� ����������
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


// �������-����� CTCPServerSocket


void CTCPServerSocket::OnAccept(int nErrorCode)
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������
	// AfxMessageBox(LPCTSTR("����������"),0,0);
    m_pDlg->OnAccept();
    CAsyncSocket::OnAccept(nErrorCode);
}






void CTCPServerSocket::OnReceive(int nErrorCode)
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������
	//int socketNumber=this->m_hSocket;
	m_pDlg->m_pConnectSocket=this;
	m_pDlg->OnReceive();
	
	CAsyncSocket::OnReceive(nErrorCode);
}


void CTCPServerSocket::SetParentDlg(CTcpServerDlg *pDlg)
{
    m_pDlg=pDlg; //��������� �� ���������� ����
}



