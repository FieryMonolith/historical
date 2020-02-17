// Lab2.h : main header file for the Lab2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"       // main symbols


// CLab2App:
// See Lab2.cpp for the implementation of this class
//

class CLab2App : public CWinApp
{
public:
	CLab2App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLab2App theApp;