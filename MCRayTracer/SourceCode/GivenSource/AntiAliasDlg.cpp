// AntiAliasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "AntiAliasDlg.h"


// CAntiAliasDlg dialog

IMPLEMENT_DYNAMIC(CAntiAliasDlg, CDialog)
CAntiAliasDlg::CAntiAliasDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAntiAliasDlg::IDD, pParent)
	, m_iterations(0)
{
}

CAntiAliasDlg::~CAntiAliasDlg()
{
}

void CAntiAliasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAntiAliasDlg, CDialog)
END_MESSAGE_MAP()


// CAntiAliasDlg message handlers
