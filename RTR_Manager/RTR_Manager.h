
// RTR_Manager.h : RTR_Manager 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#define LOG_PATH _T("C:\\R2RSet\\Log\\")

// CRTR_ManagerApp:
// 이 클래스의 구현에 대해서는 RTR_Manager.cpp을 참조하십시오.
//

class CRTR_ManagerApp : public CWinApp
{
public:
	CRTR_ManagerApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRTR_ManagerApp theApp;
