#pragma once


// CAntiADlg dialog

class CAntiADlg : public CDialog
{
	DECLARE_DYNAMIC(CAntiADlg)

public:
	CAntiADlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAntiADlg();

// Dialog Data
	enum { IDD = IDD_ANTIADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iterations;
};
