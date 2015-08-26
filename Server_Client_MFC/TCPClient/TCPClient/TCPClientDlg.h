
// TCPClientDlg.h : файл заголовка
//

#pragma once
#include "TCPClientSocket.h"
#include "afxwin.h"

// диалоговое окно CTCPClientDlg
class CTCPClientDlg : public CDialogEx
{
// Создание
public:
	CTCPClientDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TCPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_sData;
	CTCPClientSocket m_pConnectSocket; //Передающий сокет
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
