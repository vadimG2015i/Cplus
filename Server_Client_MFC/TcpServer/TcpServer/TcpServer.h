
// TcpServer.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTcpServerApp:
// � ���������� ������� ������ ��. TcpServer.cpp
//

class CTcpServerApp : public CWinApp
{
public:
	CTcpServerApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTcpServerApp theApp;