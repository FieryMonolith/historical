#pragma once


// CAntiADlg dialog

class CMonteDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonteDlg)

public:
	CMonteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonteDlg();

// Dialog Data
	enum { IDD = IDD_MONTEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iterations;
};
