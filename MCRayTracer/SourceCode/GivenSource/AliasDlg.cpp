// AliasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab2.h"
#include "AliasDlg.h"


// CAliasDlg dialog

IMPLEMENT_DYNAMIC(CAliasDlg, CDialog)
CAliasDlg::CAliasDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAliasDlg::IDD, pParent)
	, m_iter(0)
{
}

CAliasDlg::~CAliasDlg()
{
}

void CAliasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_iter);
}


BEGIN_MESSAGE_MAP(CAliasDlg, CDialog)
//	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
END_MESSAGE_MAP()


// CAliasDlg message handlers

void CAliasDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
