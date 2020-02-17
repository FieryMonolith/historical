// AntiADlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "AntiADlg.h"


// CAntiADlg dialog

IMPLEMENT_DYNAMIC(CAntiADlg, CDialog)
CAntiADlg::CAntiADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAntiADlg::IDD, pParent)
	, m_iterations(0)
{
}

CAntiADlg::~CAntiADlg()
{
}

void CAntiADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ITERATE, m_iterations);
}


BEGIN_MESSAGE_MAP(CAntiADlg, CDialog)
END_MESSAGE_MAP()


// CAntiADlg message handlers
