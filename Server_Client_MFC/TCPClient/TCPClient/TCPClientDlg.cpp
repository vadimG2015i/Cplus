
// TCPClientDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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


// диалоговое окно CTCPClientDlg




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


// обработчики сообщений CTCPClientDlg

BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	try
	{
		CStdioFile file;
	CString fileName=_T("changelog.log");
	CString m_stext;
	BOOL bOpenOK;

	CFileStatus status;
	if( CFile::GetStatus( (LPCTSTR)fileName, status ) )
	{
    // Открываем файл не создавая его
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	else
	{
    // Создаём файл и открываем его
    bOpenOK = file.Open((LPCTSTR) fileName, 
                    CFile::modeCreate | CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	
	m_fileSize = file.SeekToEnd();
	file.Close();

	m_pConnectSocket.SetParentDlg(this);
    bool res=m_pConnectSocket.Create();
	m_Seconds = 0;
    SetTimer(0, 10000, NULL);//каждые 10 сек опрос сервера
	m_ipAddress="127.0.0.1";
	UpdateData(FALSE);
	}
	 catch(CException* exception)
    {
      exception->ReportError();
    }
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTCPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTCPClientDlg::OnBnClickedConnect()
{
	// TODO: добавьте свой код обработчика уведомлений
	 //Подсоединяемся к серверу
	
	UpdateData(TRUE);
    bool res=m_pConnectSocket.Connect(m_ipAddress, 2000);

	
}






void CTCPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	 //UpdateData(FALSE);
	try
	{
	char *pBuf=new char[10];
	ltoa(m_fileSize,pBuf,10);	
	int size= m_pConnectSocket.Send(pBuf, 10); //Пересылаем
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
    // Открываем файл не создавая его
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	else
	{
    // Создаём файл и открываем его
    bOpenOK = file.Open((LPCTSTR) fileName, 
                    CFile::modeCreate | CStdioFile::modeReadWrite|CStdioFile::shareDenyNone |CStdioFile::typeBinary);
	}
	
	
	char *pBuf=new char[32768];
    //Записываем полученные данные в pBuf
    //Записываем в n количество полученных байтов
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