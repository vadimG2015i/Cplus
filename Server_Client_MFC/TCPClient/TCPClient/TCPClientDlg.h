
// TCPClientDlg.h : ���� ���������
//

#pragma once
#include "TCPClientSocket.h"
#include "afxwin.h"

// ���������� ���� CTCPClientDlg
class CTCPClientDlg : public CDialogEx
{
// ��������
public:
	CTCPClientDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_TCPCLIENT_DIALOG };

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
	CString m_sData;
	CTCPClientSocket m_pConnectSocket; //���������� �����
	afx_msg void OnBnClickedConnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnReceive();
private:
	int m_Seconds;

public:
	CString m_ipAddress;
	ULONGLONG m_fileSize;
	
	CListBox m_ListLog;
};
