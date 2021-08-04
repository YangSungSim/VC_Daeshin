// StockMstDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlusSamples.h"
#include "StockMstDlg.h"


// CStockMstDlg dialog

IMPLEMENT_DYNAMIC(CStockMstDlg, CDialog)

CStockMstDlg::CStockMstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockMstDlg::IDD, pParent)
	, m_sCode(_T(""))
{

	m_pStockCur = NULL;
	m_bIsSB = false;
}

CStockMstDlg::~CStockMstDlg()
{
}

void CStockMstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_CODE, m_sCode);
	DDV_MaxChars(pDX, m_sCode, 7);
}


BEGIN_MESSAGE_MAP(CStockMstDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CStockMstDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CStockMstDlg message handlers

void CStockMstDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	USES_CONVERSION;



	//IDibPtr pStockMst = NULL;
	CString strErr = _T("통신에러: ");

	// 종목코드 정상 확인
	CString strCode = m_sCode;
	if (strCode == "")
		strCode = "A003540";
	strCode.MakeUpper();	// 대문자로 만들기 
	if (strCode[0] != 'A')
		strCode = "A" + strCode;


	//IDibPtr pStockMst;
	CComPtr<DSCBO1Lib::IDib> pStockMst  = NULL;
	HRESULT hr;

	// 통신 객체 생성 
	if( !pStockMst )
	{
		hr = pStockMst.CoCreateInstance(__uuidof(DSCBO1Lib::StockMst));
		if (FAILED(hr))
		{
			AfxMessageBox("(DSCBO1Lib::StockMst 생성 실패");
			return;
		}
	}
		
	// 실시간 sb가 있을 경우 cancel 
	if (m_pStockCur)
	{
		if (m_bIsSB)
			m_pStockCur->Unsubscribe();
	}

	//CComBSTR bstInput = ;
	CComVariant varInput = strCode;
	pStockMst->SetInputValue(0, varInput);
	hr = pStockMst->BlockRequest();

	if (FAILED(hr))
	{
		AfxMessageBox("BlockRequest 실패");
		return;
	}

	if (0 != pStockMst->GetDibStatus() )
	{
		AfxMessageBox("통신 오류");
		return;
	}

	
	// 현재가 주요 값
	// 1 - 종목명
	// 11 - 현재가
	// 13 - 시가
	// 14 - 고가
	// 15 - 저가 
	CString sResult;
	CComVariant varRet1 = pStockMst->GetHeaderValue(1);
	CComVariant varRet11 = pStockMst->GetHeaderValue(11);
	CComVariant varRet13 = pStockMst->GetHeaderValue(13);
	CComVariant varRet14 = pStockMst->GetHeaderValue(14);
	CComVariant varRet15 = pStockMst->GetHeaderValue(15);

	sResult.Format("종목명 - %s\n현재가 - %d\n시가 - %d\n고가- %d\n저가- %d\n",W2A(varRet1.bstrVal),varRet11.lVal, varRet13.lVal,varRet14.lVal,varRet15.lVal);
	TRACE(sResult);

	// 실시간 sb 요청하기 
	if (!m_pStockCur)
	{
		hr = m_pStockCur.CoCreateInstance(__uuidof(DSCBO1Lib::StockCur));
		if (FAILED(hr))
		{
			AfxMessageBox("(DSCBO1Lib::StockMst 생성 실패");
			return;
		}
		hr = DispEventAdvise((IUnknown*)m_pStockCur);
		if (FAILED(hr))
		{
			AfxMessageBox("DispEventAdvise 실패");
			return;
		}
	}
	m_pStockCur->SetInputValue(0, varInput);
	m_pStockCur->Subscribe();
	m_bIsSB = true;

}

// Event
void __stdcall CStockMstDlg::Received()
{
	CComVariant varCur = m_pStockCur->GetHeaderValue(13);
	CComVariant varDiff = m_pStockCur->GetHeaderValue(2);
	TRACE("현재가 %d, 대비 %d\n", varCur.lVal, varDiff.lVal);
}


void CStockMstDlg::OnDestroy()
{
	__super::OnDestroy();
	if (m_pStockCur)
	{
		if (m_bIsSB)
			m_pStockCur->Unsubscribe();
		DispEventUnadvise(m_pStockCur);
	}

}
