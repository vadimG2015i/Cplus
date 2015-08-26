
// TcpServer.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTcpServerApp:
// О реализации данного класса см. TcpServer.cpp
//

class CTcpServerApp : public CWinApp
{
public:
	CTcpServerApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTcpServerApp theApp;