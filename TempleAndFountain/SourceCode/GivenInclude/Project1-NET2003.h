// Project1-NET2003.h : main header file for the Project1-NET2003 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CProject1NET2003App:
// See Project1-NET2003.cpp for the implementation of this class
//

class CProject1NET2003App : public CWinApp
{
public:
	CProject1NET2003App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CProject1NET2003App theApp;