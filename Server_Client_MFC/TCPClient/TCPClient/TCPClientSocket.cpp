// TCPClientSocket.cpp: ���� ����������
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




// �������-����� CTCPClientSocket


void CTCPClientSocket::SetParentDlg(CTCPClientDlg *pDlg)
{
    m_pDlg=pDlg; //������ ��������� �� ���������� ����
}

void CTCPClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �������� ������������������ ��� ��� ����� �������� ������
	m_pDlg->OnReceive();

	CAsyncSocket::OnReceive(nErrorCode);
}
