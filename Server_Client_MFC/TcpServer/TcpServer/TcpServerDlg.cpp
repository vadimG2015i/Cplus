
// TcpServerDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TcpServer.h"
#include "TcpServerDlg.h"
#include "afxdialogex.h"
#include "TCPServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT   MFCThreadFunc (LPVOID param ) ;
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


// диалоговое окно CTcpServerDlg




CTcpServerDlg::CTcpServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpServerDlg::IDD, pParent)
	, m_Folder(_T(""))
	, m_sData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_Folder);
	DDX_Control(pDX, IDC_FILES, m_listFiles);
	DDX_Text(pDX, IDC_EDIT1, m_sData);
}

BEGIN_MESSAGE_MAP(CTcpServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LISTEN, &CTcpServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_FOLDER, &CTcpServerDlg::OnBnClickedFolder)
END_MESSAGE_MAP()


// обработчики сообщений CTcpServerDlg

BOOL CTcpServerDlg::OnInitDialog()
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
	m_pListenSocket.SetParentDlg(this);
	m_pConnectSocket=new CTCPServerSocket;
    m_pConnectSocket->SetParentDlg(this);
    bool res=m_pListenSocket.Create(2000); //Создаем сокет на 2000 порту
		return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTcpServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpServerDlg::OnPaint()
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
HCURSOR CTcpServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTcpServerDlg::OnAccept()
{
	try
	{
	CString ipAddress;
	UINT ipPort;
	char tempBuf [ 32 ] = { 0 };
    //Переводим соединение на передающий сокет
	CTCPServerSocket *m_pConnectSocketN=new CTCPServerSocket;
	m_pConnectSocketN->SetParentDlg(this);
    bool res= m_pListenSocket.Accept(*m_pConnectSocketN);
	m_pConnectSocketN->GetPeerName(ipAddress,ipPort);
	ipAddress+=" ";
	ipAddress+=CString(itoa(ipPort,tempBuf,10));
	m_listFiles.AddString(ipAddress);
	UpdateData(FALSE);
	}
	 catch(CException* exception)
    {
      exception->ReportError();
    }

}

void CTcpServerDlg::OnListen()
{
    // TODO: Add your control notification handler code here
    m_pListenSocket.Listen(); //Сокет начинает слушать
}


void CTcpServerDlg::OnReceive()
{
	try
	{
			CString ipAddress;
	char *pBuf=new char[64];
	//Записываем полученные данные в pBuf
    //Записываем в n количество полученных байтов
    int n=m_pConnectSocket->Receive(pBuf, 64);
    //Отсекаем лишние символы
    pBuf[n]=NULL;
	ULONGLONG size_client= atol(pBuf);
	CStdioFile file;
	CString fileName=_T("changelog.log");
	BOOL bOpenOK;
	// Открываем файл не создавая его
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CFile::modeReadWrite|CStdioFile::shareDenyNone|CStdioFile::typeBinary);
	
	ULONGLONG lActual = file.Seek(size_client,CStdioFile::begin);

	char *pBufSend=new char[32768];

	CString stext;
 	CString buffer;
 	while (file.ReadString(stext) != FALSE)
 	{
		if(buffer.GetLength()>32000) break;
 		buffer += stext;
 		buffer += "\n";
		
  	}
	file.Close();
	int size= m_pConnectSocket->Send(buffer, buffer.GetLength()); //Пересылаем
     UpdateData(FALSE);
}
	  catch(CException* exception)
    {
      exception->ReportError();
    }
	}

void CTcpServerDlg::OnBnClickedListen()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_pListenSocket.Listen(); //Сокет начинает слушать
}


void CTcpServerDlg::OnBnClickedFolder()


{
	// TODO: добавьте свой код обработчика уведомлений
	try
	{
	m_Folder="";
	TCHAR sizezDir[MAX_PATH];
	BROWSEINFO	bffi = {m_hWnd, 0, sizezDir, _T("Please, select a folder"), BIF_USENEWUI, 0, 0, 0};
	LPITEMIDLIST	pIDl(0);
	pIDl = SHBrowseForFolder(&bffi);
	SHGetPathFromIDList(pIDl, sizezDir);
	m_Folder.Append(sizezDir);
	m_Folder.Append(_T("\\"));
	UpdateData(FALSE);
	AfxBeginThread(MFCThreadFunc,(LPVOID)&m_Folder,THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	 catch(CException* exception)
    {
      exception->ReportError();
    }
	//::MessageBox((HWND)param, (LPCWSTR)"Thread activated", (LPCWSTR)"Message from Thread" ,MB_OK);
}




UINT  MFCThreadFunc(LPVOID param)
{
	
	try

	{
	CString* tmpFolderName = static_cast<CString*>(param);
	CStringA strA(*tmpFolderName); // a helper string
	CStdioFile file;
	CString fileName=_T("changelog.log");
	BOOL bOpenOK;

	CFileStatus status;
	if( CFile::GetStatus( (LPCTSTR)fileName, status ) )
	{
    // Открываем файл не создавая его
    bOpenOK = file.Open( (LPCTSTR)fileName, 
                    CStdioFile::modeReadWrite|CStdioFile::shareDenyNone|CStdioFile::typeBinary);
	}
	else
	{
    // Создаём файл и открываем его
    bOpenOK = file.Open((LPCTSTR) fileName, 
                    CStdioFile::modeCreate | CStdioFile::modeReadWrite|CFile::shareDenyNone|CStdioFile::typeBinary);
	}
	file.SeekToEnd();
	CString strValue;
	 DWORD cbOffset;
  	LPCSTR folderName = strA;
 
	HANDLE hDir = CreateFileA(folderName,
					  FILE_LIST_DIRECTORY,
					  FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,
					  NULL,
					  OPEN_EXISTING,
					  FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
					  NULL);
 
		 // if (hDir == INVALID_HANDLE_VALUE) 
	int lastError = GetLastError();	

	OVERLAPPED o;
	o.hEvent=CreateEventA(0, true, false, 0);
 
	BYTE outBuffer[5120];
	VOID *pBuf = (BYTE*)&outBuffer;
	DWORD InfoNotify;
	BOOL ResultReadChange;
	DWORD outSize = sizeof(outBuffer);
 
 
	while(1)
		 {
		  ZeroMemory(outBuffer,sizeof(outBuffer));
		 ResultReadChange = ReadDirectoryChangesW(hDir,
							 &outBuffer,
							 outSize,
							 TRUE,
							 FILE_NOTIFY_CHANGE_SIZE|
							 FILE_NOTIFY_CHANGE_DIR_NAME|
							 FILE_NOTIFY_CHANGE_FILE_NAME,
							 &InfoNotify,
							 &o,
							 NULL);
 
		  if(WaitForSingleObject(o.hEvent,INFINITE)==WAIT_OBJECT_0)
		  {
		   PFILE_NOTIFY_INFORMATION fni = (PFILE_NOTIFY_INFORMATION)pBuf;
		   CString temp_patch(folderName);
					do
					{
						cbOffset = fni->NextEntryOffset;
						strValue="";
						ZeroMemory(fni,sizeof(fni));

						CString action;// = "";
					

						if( fni->Action == FILE_ACTION_ADDED)
							strValue =CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ")+CString("File add ") + temp_patch+CString("\\")+CString(fni->FileName);
						
												
						else if( fni->Action == FILE_ACTION_REMOVED)
							strValue =CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ")+CString("File removed ") + temp_patch+CString("\\")+CString(fni->FileName);
						
						else if( fni->Action == FILE_ACTION_MODIFIED)
							strValue =CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ")+CString("File modified ") + temp_patch+CString("\\")+CString(fni->FileName);
							

						
						else if( fni->Action == FILE_ACTION_RENAMED_OLD_NAME)
							strValue =CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ")+CString("File renamed  Old name :") + temp_patch+CString("\\")+CString(fni->FileName);
							
						
						else if( fni->Action == FILE_ACTION_RENAMED_NEW_NAME)
							strValue =CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S ")+CString("File renamed New name  ") + temp_patch+CString("\\")+CString(fni->FileName);
							
						
						strValue.Append(CString("\n"));
						file.WriteString(strValue);
						fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE) fni + cbOffset);
						file.Flush();
					} while( cbOffset );
 
		}
	 }
	 //::MessageBox((HWND)param, (LPCWSTR)"Thread activated", (LPCWSTR)"Message from Thread" ,MB_OK);
	 file.Close();
	 }
	  catch(CException* exception)
    {
      exception->ReportError();
    }
	return 0 ;
}



