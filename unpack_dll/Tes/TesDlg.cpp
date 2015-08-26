// TesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tes.h"
#include "TesDlg.h"
#include "../unpack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesDlg dialog

CTesDlg::CTesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTesDlg)
	m_Path = _T("");
	m_Edit = _T("");
	m_Packfile = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTesDlg)
	DDX_Control(pDX, IDC_LIST2, m_List);
	DDX_Text(pDX, IDC_SOURCE, m_Path);
	DDX_Text(pDX, IDC_PASS, m_Packfile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTesDlg, CDialog)
	//{{AFX_MSG_MAP(CTesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnUnrar)
	ON_BN_CLICKED(IDC_BUTTON5, OnUnarj)
	ON_BN_CLICKED(IDC_BUTTON6, OnUnace)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_TAR, OnTar)
	ON_BN_CLICKED(IDC_LHA, OnLha)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesDlg message handlers

BOOL CTesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTesDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTesDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog dlgPath(NULL,NULL,NULL,OFN_EXPLORER , szFilter,this);;
	dlgPath.DoModal();
	m_Path=dlgPath.GetPathName();
	CString temp=dlgPath.GetFileName();
	m_Path.Delete(m_Path.GetLength()-temp.GetLength(),temp.GetLength());
		this->UpdateData(FALSE);
	
}

void CTesDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Archive Files (*.zip,*.rar,*.arj,*.ace)|*.*|SFX  Files (*.exe)|*.exe|All Files (*.*)|*.*||";
	
	CFileDialog dlgSource(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,this);
	dlgSource.DoModal();
	m_Packfile=dlgSource.GetPathName();
	//m_Path=dlgSource.GetFolderPath();
	this->UpdateData(FALSE);

}

void CTesDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	//this->UpdateData(FALSE);
	HANDZIP file;
	BOOL success=FALSE;
		char * name=(char*)malloc(260);
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	file=Zip_OpenArchive(m_Packfile);
	success=Zip_GetFirstFile(file,m_Path,name);
	m_Edit=name;
	//name=ZipFileName;
		this->UpdateData(FALSE);
	while (success)
	{
		success=Zip_GetNextFile(file,m_Path,name);
	
			
			m_List.AddString(name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	Zip_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
	
}

void CTesDlg::OnUnrar() 
{
	// TODO: Add your control notification handler code here

	this->UpdateData(TRUE);
	this->UpdateData(FALSE);
	HANDRAR file;
		//HANDRAR file1;
	
	BOOL success=FALSE;
	char* name=(char*)malloc(1024);;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	 file=Rar_OpenArchive(m_Packfile,m_Path);
	 if (!file) return;
	success=Rar_GetFirstFile(file,name);
	m_Edit=name;
	//name=ZipFileName;
		this->UpdateData(FALSE);
	while (success)
	{
		m_List.AddString(name);
		if (success=Rar_GetNextFile(file,name))
	
			
			;
		//	name="";
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};

	Rar_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
	
}

void CTesDlg::OnUnarj() 
{
	// TODO: Add your control notification handler code here


		this->UpdateData(TRUE);
	this->UpdateData(FALSE);
	HANDARJ file;
		char * name=(char*)malloc(260);;
		BOOL success=FALSE;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	file=Arj_OpenArchive(m_Packfile);
if (success	) m_List.AddString((const char*)name);
		success=Arj_GetFirstFile(file,m_Path,name);
	if (success) m_List.AddString((const char*)name);
	m_Edit=name;

	while (success)
	{
		success=Arj_GetNextFile(file,m_Path,name);
		
		if (success	)
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	Arj_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
	
}

void CTesDlg::OnUnace() 
{
	// TODO: Add your control notification handler code here

		this->UpdateData(TRUE);
	this->UpdateData(FALSE);
	HANDACE file;
		char * name=(char*)malloc(260);;
		BOOL success=FALSE;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	file=Ace_OpenArchive(m_Packfile);
	success=Ace_GetFirstFile(file,m_Path,name);
	if (strlen(name)) m_List.AddString((const char*)name);
	m_Edit=name;
	
	while (strlen(name))
	{
		success=Ace_GetNextFile(file,m_Path,name);
		
		if (strlen(name))
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	Ace_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
	
	
}

void CTesDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	this->UpdateData(FALSE);
    HANDCABMS file;
		char * name=(char*)malloc(260);;
		BOOL success=FALSE;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	 file=Cab_OpenArchive(m_Packfile);
	success=Cab_GetFirstFile(file,m_Path,name);
	if (strlen(name)) m_List.AddString((const char*)name);
	m_Edit=name;
	//name=ZipFileName;
	//this->UpdateData(FALSE);
	while (success)
	{
		success=Cab_GetNextFile(file,m_Path,name);
		//CString temp=InZipFileName;
		if (success)
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	Cab_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
}

void CTesDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	

	this->UpdateData(TRUE);
	this->UpdateData(FALSE);
    HANDCABIS file;
		char * name=(char*)malloc(260);;
		BOOL success=FALSE;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
 	AfxGetApp()->DoWaitCursor(1);
	 file=CabIS_OpenArchive(m_Packfile,m_Path);
	success=CabIS_GetFirstFile(file,name);
	if (success) m_List.AddString((const char*)name);
	m_Edit=name;
	//name=ZipFileName;
	//this->UpdateData(FALSE);
	while (success)
	{
		success=CabIS_GetNextFile(file,name);
		//CString temp=InZipFileName;
		if (success)
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	CabIS_CloseArchive(file);
	AfxGetApp()->DoWaitCursor(-1);

end: ;
}

void CTesDlg::OnTar() 
{
	// TODO: Add your control notification handler code here
		this->UpdateData(TRUE);
	this->UpdateData(FALSE);
    HANDTAR file;
		char * name=(char*)malloc(520);;
		BOOL success=FALSE;

	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	 file=Tar_OpenArchive(m_Packfile,m_Path);
	 if (file==NULL) return;

	 success=Tar_GetFirstFile(file,name);
	if (success) m_List.AddString((const char*)name);
	m_Edit=name;
	
	while (success)
	{
		success=Tar_GetNextFile(file,name);
		//CString temp=InZipFileName;
		if (success)
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	

		Tar_CloseArchive(file);
		end:;
}

void CTesDlg::OnLha() 
{
	// TODO: Add your control notification handler code here

		this->UpdateData(TRUE);
	this->UpdateData(FALSE);
    HANDLHA file;
		char * name=(char*)malloc(520);;
		BOOL success=FALSE;
	//m_Path="c://temp/";
	if (m_Packfile.IsEmpty() || m_Path.IsEmpty()) 
	{ AfxMessageBox("One or two field is empty!!",MB_OK,NULL);
	 goto end;
	}
	;
	AfxGetApp()->DoWaitCursor(1);
	 file=LHA_OpenArchive(m_Packfile,m_Path);
	 if (file==NULL) return;

	 success=LHA_GetFirstFile(file,name);
	if (success) m_List.AddString((const char*)name);
	m_Edit=name;
	
	while (success)
	{
		success=LHA_GetNextFile(file,name);
	
		if (success)
			m_List.AddString((const char*)name);
			this->UpdateData(FALSE);
			this->UpdateData(TRUE);
	};
	
	LHA_CloseArchive(file);
	end:
	;
	
}
