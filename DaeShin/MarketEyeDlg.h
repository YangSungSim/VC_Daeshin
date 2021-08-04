#pragma once


// CMarketEyeDlg dialog

class CMarketEyeDlg : public CDialog
{
	DECLARE_DYNAMIC(CMarketEyeDlg)

public:
	CMarketEyeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMarketEyeDlg();

// Dialog Data
	enum { IDD = IDD_MAKETEYE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
