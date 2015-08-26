
// TcpServerDlg.h : ���� ���������
//

#pragma once

//class CTCPServerSocket;
#include "TCPServerSocket.h"
#include "afxwin.h"

// ���������� ���� CTcpServerDlg
class CTcpServerDlg : public CDialogEx
{
// ��������
public:
	CTcpServerDlg(CWnd* pParent = NULL);	// ����������� �����������

	CTCPServerSocket m_pListenSocket;//=new CTCPServerSocket; //��������� �����
    CTCPServerSocket *m_pConnectSocket;//=new CTCPServerSocket;; //���������� �����
	void OnAccept();
	void OnListen();
	void OnReceive();
	
   
// ������ ����������� ����
	enum { IDD = IDD_TCPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedListen();
	CString m_Folder;
	afx_msg void OnBnClickedFolder();
	CListBox m_listFiles;
	CString m_sData;
	volatile BOOL  worked_thread;
};
