// AntSkeleton.h : main header file for the AntSkeleton application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CAntSkeletonApp:
// See AntSkeleton.cpp for the implementation of this class
//

class CAntSkeletonApp : public CWinApp
{
public:
	CAntSkeletonApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAntSkeletonApp theApp;