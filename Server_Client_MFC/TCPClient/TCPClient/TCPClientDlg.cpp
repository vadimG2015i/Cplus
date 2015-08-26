
// TCPClientDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���������� ���� CAboutDlg ������������ ��� �������� �������� � ����������

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ������ ����������� ����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

// ����������
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ���������� ���� CTCPClientDlg




CTCPClientDlg::CTCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPClientDlg::IDD, pParent)
	, m_sData(_T(""))
	, m_ipAddress(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ipAddress);

	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
}

BEGIN_MESSAGE_MAP(CTCPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CTCPClientDlg::OnBnClickedConnect)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ����������� ��������� CTCPClientDlg

BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���������� ������ "� ���������..." � ��������� ����.

	// IDM_ABOUTBOX ������ ���� � �������� ��������� �������.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������
	try
	{
		CStdioFile file;
	CString fileName=_T("changelog.log");
	CString m_stext;
	BOOL bOpenOK;

	CFileStatus status;
	if( CFile::GetStatus( (LPCTSTR)fileName, status ) )
	{
    // ��������� ���� �� �������� ���
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	else
	{
    // ������ ���� � ��������� ���
    bOpenOK = file.Open((LPCTSTR) fileName, 
                    CFile::modeCreate | CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	
	m_fileSize = file.SeekToEnd();
	file.Close();

	m_pConnectSocket.SetParentDlg(this);
    bool res=m_pConnectSocket.Create();
	m_Seconds = 0;
    SetTimer(0, 10000, NULL);//������ 10 ��� ����� �������
	m_ipAddress="127.0.0.1";
	UpdateData(FALSE);
	}
	 catch(CException* exception)
    {
      exception->ReportError();
    }
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

void CTCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CTCPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CTCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTCPClientDlg::OnBnClickedConnect()
{
	// TODO: �������� ���� ��� ����������� �����������
	 //�������������� � �������
	
	UpdateData(TRUE);
    bool res=m_pConnectSocket.Connect(m_ipAddress, 2000);

	
}






void CTCPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �������� ���� ��� ����������� ��������� ��� ����� ������������
	 //UpdateData(FALSE);
	try
	{
	char *pBuf=new char[10];
	ltoa(m_fileSize,pBuf,10);	
	int size= m_pConnectSocket.Send(pBuf, 10); //����������
	delete pBuf;
	}
	 catch(CException* exception)
    {
      exception->ReportError();
    }
	CDialogEx::OnTimer(nIDEvent);
}

void CTCPClientDlg::OnReceive()
	{

try
{
	CStdioFile file;
	CString fileName=_T("changelog.log");
	CString m_stext;
	BOOL bOpenOK;

	CFileStatus status;
	if( CFile::GetStatus( (LPCTSTR)fileName, status ) )
	{
    // ��������� ���� �� �������� ���
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	else
	{
    // ������ ���� � ��������� ���
    bOpenOK = file.Open((LPCTSTR) fileName, 
                    CFile::modeCreate | CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	
	
	char *pBuf=new char[32768];
    //���������� ���������� ������ � pBuf
    //���������� � n ���������� ���������� ������
    int n=m_pConnectSocket.Receive(pBuf, 32768);
    file.SeekToEnd();
	file.Write(pBuf,n);
	file.Flush();
	ULONGLONG lActual = file.Seek(m_fileSize,CStdioFile::begin);
	int sizeList=0;
	
 	while (file.ReadString(m_stext) != FALSE)
 	{
		if (sizeList >250)
			 sizeList= m_ListLog.DeleteString(0);
		int sizeList=m_ListLog.AddString(m_stext);
		 
		
 	
 	}
	UpdateData(FALSE);
	m_fileSize = file.SeekToEnd();
	file.Close();
	delete pBuf;
}
	 catch(CException* exception)
    {
      exception->ReportError();
    }

	};