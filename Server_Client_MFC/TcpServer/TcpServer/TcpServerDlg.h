
// TcpServerDlg.h : файл заголовка
//

#pragma once

//class CTCPServerSocket;
#include "TCPServerSocket.h"
#include "afxwin.h"

// диалоговое окно CTcpServerDlg
class CTcpServerDlg : public CDialogEx
{
// Создание
public:
	CTcpServerDlg(CWnd* pParent = NULL);	// стандартный конструктор

	CTCPServerSocket m_pListenSocket;//=new CTCPServerSocket; //Слушающий сокет
    CTCPServerSocket *m_pConnectSocket;//=new CTCPServerSocket;; //Передающий сокет
	void OnAccept();
	void OnListen();
	void OnReceive();
	
   
// Данные диалогового окна
	enum { IDD = IDD_TCPSERVER_DIALOG };

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
	afx_msg void OnBnClickedListen();
	CString m_Folder;
	afx_msg void OnBnClickedFolder();
	CListBox m_listFiles;
	CString m_sData;
	volatile BOOL  worked_thread;
};
