// Tes.h : main header file for the TES application
//

#if !defined(AFX_TES_H__BB087A7E_2CB1_4C98_B6DA_D8274FF6A138__INCLUDED_)
#define AFX_TES_H__BB087A7E_2CB1_4C98_B6DA_D8274FF6A138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTesApp:
// See Tes.cpp for the implementation of this class
//

class CTesApp : public CWinApp
{
public:
	CTesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TES_H__BB087A7E_2CB1_4C98_B6DA_D8274FF6A138__INCLUDED_)
