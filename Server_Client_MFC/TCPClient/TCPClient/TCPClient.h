
// TCPClient.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTCPClientApp:
// � ���������� ������� ������ ��. TCPClient.cpp
//

class CTCPClientApp : public CWinApp
{
public:
	CTCPClientApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTCPClientApp theApp;