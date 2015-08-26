
// TCPClient.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTCPClientApp:
// О реализации данного класса см. TCPClient.cpp
//

class CTCPClientApp : public CWinApp
{
public:
	CTCPClientApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTCPClientApp theApp;