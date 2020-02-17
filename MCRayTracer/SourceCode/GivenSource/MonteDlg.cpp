// MonteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "MonteDlg.h"


// CMonteDlg dialog

IMPLEMENT_DYNAMIC(CMonteDlg, CDialog)
CMonteDlg::CMonteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonteDlg::IDD, pParent)
	, m_iterations(1)
{
}

CMonteDlg::~CMonteDlg()
{
}

void CMonteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ITERATE, m_iterations);
}


BEGIN_MESSAGE_MAP(CMonteDlg, CDialog)
END_MESSAGE_MAP()


// CMonteDlg message handlers
