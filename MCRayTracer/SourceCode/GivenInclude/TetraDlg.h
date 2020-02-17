#pragma once


// CTetraDlg dialog

class CTetraDlg : public CDialog
{
	DECLARE_DYNAMIC(CTetraDlg)

public:
	CTetraDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTetraDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_size;
};
