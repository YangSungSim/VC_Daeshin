#pragma once


// CStockBidDlg 대화 상자

class CStockBidDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStockBidDlg)

public:
	CStockBidDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStockBidDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STOCK_BID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	CString m_sCode;

};
