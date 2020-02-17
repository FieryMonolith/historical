#pragma once

// CAntiAliasDlg dialog

class CAntiAliasDlg : public CDialog
{
	DECLARE_DYNAMIC(CAntiAliasDlg)

public:
	CAntiAliasDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAntiAliasDlg();
	
	int GetIterations() { return m_iterations; }

// Dialog Data
	//enum { IDD = IDD_ANTIALIAS };
	enum { IDD = IDD_Alias };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_iterations;
};
