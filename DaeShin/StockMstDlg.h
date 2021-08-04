#pragma once


// CStockMstDlg dialog

static _ATL_FUNC_INFO OnEventInfo[] =
{
	{CC_STDCALL, VT_EMPTY, 0, {}},
};

#define ID_EVENT_DIB		701

class CStockMstDlg : 
	public CDialog,
	public IDispEventSimpleImpl<ID_EVENT_DIB, CStockMstDlg, &__uuidof(DSCBO1Lib::_IDibEvents) >
{
	DECLARE_DYNAMIC(CStockMstDlg)

public:
	CStockMstDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStockMstDlg();

// Dialog Data
	enum { IDD = IDD_STOCK_MST };

	BEGIN_SINK_MAP(CStockMstDlg)
		SINK_ENTRY_INFO(ID_EVENT_DIB, __uuidof(DSCBO1Lib::_IDibEvents), 0x00000001, Received, &OnEventInfo[0])
	END_SINK_MAP();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CComPtr<DSCBO1Lib::IDib> m_pStockCur;
	CString m_sCode;
	bool m_bIsSB;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();

	void _stdcall Received();


};
