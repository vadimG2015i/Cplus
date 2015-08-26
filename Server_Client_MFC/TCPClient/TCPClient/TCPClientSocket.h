#pragma once

// конечный объект команды CTCPClientSocket
class CTCPClientDlg;
class CTCPClientSocket : public CAsyncSocket
{
public:
	CTCPClientSocket();
	CTCPClientDlg * m_pDlg;
	virtual ~CTCPClientSocket();
	void SetParentDlg (CTCPClientDlg *pDlg);
	virtual void OnReceive(int nErrorCode);
};


