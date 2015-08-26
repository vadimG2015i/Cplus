// TesDlg.h : header file
//

#if !defined(AFX_TESDLG_H__1956F2A8_065E_44AB_B5AE_9475CF271FD3__INCLUDED_)
#define AFX_TESDLG_H__1956F2A8_065E_44AB_B5AE_9475CF271FD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTesDlg dialog

class CTesDlg : public CDialog
{
// Construction
public:
	CTesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTesDlg)
	enum { IDD = IDD_TES_DIALOG };
	CListBox	m_List;
	CString	m_Path;
	CString	m_Edit;
	CString	m_Packfile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnUnrar();
	afx_msg void OnUnarj();
	afx_msg void OnUnace();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnTar();
	afx_msg void OnLha();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESDLG_H__1956F2A8_065E_44AB_B5AE_9475CF271FD3__INCLUDED_)
