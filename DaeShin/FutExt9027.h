#pragma once


// CFutExt9027 대화 상자

static _ATL_FUNC_INFO OnEventInfo2[] =
{
	{ CC_STDCALL, VT_EMPTY, 0,{} },
};
#define ID_EVENT_DIB2		702


class CFutExt9027 : 
		public CDialogEx,
	public IDispEventSimpleImpl<ID_EVENT_DIB2, CFutExt9027, &__uuidof(CPSYSDIBLib::_ISysDibEvents) >
{
	DECLARE_DYNAMIC(CFutExt9027)

public:
	CFutExt9027(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFutExt9027();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FUT_EXT };
#endif

	BEGIN_SINK_MAP(CFutExt9027)
		SINK_ENTRY_INFO(ID_EVENT_DIB2, __uuidof(CPSYSDIBLib::_ISysDibEvents), 0x00000001, Received, &OnEventInfo2[0])
	END_SINK_MAP();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
private:
	CString m_sCode;
	bool m_bIsSB;
	CComPtr<CPSYSDIBLib::ISysDib> m_pFutExtCur;

	afx_msg void OnDestroy();

	void _stdcall Received();

};
