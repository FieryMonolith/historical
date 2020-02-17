// TetraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "TetraDlg.h"


// CTetraDlg dialog

IMPLEMENT_DYNAMIC(CTetraDlg, CDialog)
CTetraDlg::CTetraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTetraDlg::IDD, pParent)
	, m_size(0)
{
}

CTetraDlg::~CTetraDlg()
{
}

void CTetraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT, m_size);
}


BEGIN_MESSAGE_MAP(CTetraDlg, CDialog)
END_MESSAGE_MAP()


// CTetraDlg message handlers
