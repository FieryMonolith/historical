#pragma once


// CAliasDlg dialog

class CAliasDlg : public CDialog
{
	DECLARE_DYNAMIC(CAliasDlg)

public:
	CAliasDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAliasDlg();

// Dialog Data
	enum { IDD = IDD_Alias };

	int GetIter() { return m_iter; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	int m_iter;
public:
	afx_msg void OnEnChangeEdit2();
};
