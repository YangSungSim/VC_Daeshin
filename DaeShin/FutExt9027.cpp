// CFutExt9027.cpp: 구현 파일
//

#include "stdafx.h"
#include "PlusSamples.h"
#include "FutExt9027.h"
#include "afxdialogex.h"
#include "atlsafe.h"
#include <string>
#include <vector>
using namespace std;


// CFutExt9027 대화 상자

IMPLEMENT_DYNAMIC(CFutExt9027, CDialogEx)

CFutExt9027::CFutExt9027(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FUT_EXT, pParent)
	, m_sCode(_T(""))
{
	m_pFutExtCur = NULL;
	m_bIsSB = false;

}

CFutExt9027::~CFutExt9027()
{
}

void CFutExt9027::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_CODE, m_sCode);
	DDV_MaxChars(pDX, m_sCode, 7);

}


BEGIN_MESSAGE_MAP(CFutExt9027, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFutExt9027::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFutExt9027 메시지 처리기


void CFutExt9027::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	USES_CONVERSION;



	//IDibPtr pFutExt = NULL;
	CString strErr = _T("통신에러: ");

	// 종목코드 정상 확인
	CString strCode = m_sCode;
	if (strCode == "")
		strCode = "101N6";

	//IDibPtr pFutExt;
	CComPtr<DSCBO1Lib::IDib> pFutExt = NULL;
	HRESULT hr;

	// 통신 객체 생성 
	if (!pFutExt)
	{
		hr = pFutExt.CoCreateInstance(__uuidof(DSCBO1Lib::CpSvr9027));
		if (FAILED(hr))
		{
			AfxMessageBox("(DSCBO1Lib::CpSvr9027 생성 실패");
			return;
		}
	}


	//CComBSTR bstInput = ;
	CComVariant varInput = strCode;
	pFutExt->SetInputValue(0, varInput);


	long lTotCnt = 0;
	while (True)
	{
		hr = pFutExt->BlockRequest();

		if (FAILED(hr))
		{
			AfxMessageBox("BlockRequest 실패");
			return;
		}

		if (0 != pFutExt->GetDibStatus())
		{
			AfxMessageBox("통신 오류");
			return;
		}
		_bstr_t bstrMsg = pFutExt->GetDibMsg1();
		CString sMsg = LPCSTR(bstrMsg);
		if (sMsg.GetLength() > 0)
		{
			sMsg += "\n";
			TRACE(sMsg);
		}



		int cntData = pFutExt->GetHeaderValue(0);		// 데이타 개수 

		CComVariant varCur, varDiff, varTime;


		long lTime;
		double dExCur;
		double dExDiff;

		for (int i = 0; i < cntData; i++)
		{
			varTime = pFutExt->GetDataValue(0, i);
			varCur = pFutExt->GetDataValue(1, i);
			varDiff = pFutExt->GetDataValue(2, i);


			lTime = varTime.lVal;
			dExCur= varCur.dblVal;
			dExDiff = varDiff.dblVal;

			TRACE("시간 %d 예상현재가 %.2f 예상대비 %.2f, \n", lTime, dExCur, dExDiff);
		}
		
		lTotCnt += cntData;
		// 연속 통신 가능 여부 체크 
		long lCont;
		pFutExt->get_Continue(&lCont);
		if (lCont == 0)
		{
			TRACE("연속 데이터 없음\n");
			break;
		}
		//TRACE("연속 데이터 있음\n");
		//if (lTotCnt > 500)
		//	break;
	}

	// 실시간 sb가 있을 경우 cancel 
	if (m_pFutExtCur)
	{
		if (m_bIsSB)
			m_pFutExtCur->Unsubscribe();
	}

	// 실시간 sb 요청하기 
	if (!m_pFutExtCur)
	{
		hr = m_pFutExtCur.CoCreateInstance(__uuidof(CPSYSDIBLib::FOExpectCur));
		if (FAILED(hr))
		{
			AfxMessageBox("CPSYSDIBLib::FOExpectCur 생성 실패");
			return;
		}
		hr = DispEventAdvise((IUnknown*)m_pFutExtCur);
		if (FAILED(hr))
		{
			AfxMessageBox("DispEventAdvise 실패");
			return;
		}
	}
	CComBSTR bstrMarket = "*";
	CComVariant varMarket = bstrMarket;
	m_pFutExtCur->SetInputValue(0, varInput);
	m_pFutExtCur->SetInputValue(1, varMarket);
	m_pFutExtCur->Subscribe();
	m_bIsSB = true;

	TRACE("CPSYSDIBLib::FOExpectCur 요청 성공 \n");
}

// Event
void __stdcall CFutExt9027::Received()
{
	USES_CONVERSION;

	CComVariant varCode = m_pFutExtCur->GetHeaderValue(0);
	CComVariant varTime = m_pFutExtCur->GetHeaderValue(1);
	CComVariant varExCur = m_pFutExtCur->GetHeaderValue(2);
	CComVariant varExDiff = m_pFutExtCur->GetHeaderValue(3);

	CComBSTR bstrCode = varCode.bstrVal;
	TRACE("code %s, 시간 %d, 예상체결가 %.2f, 예상체결가대비 %.2f\n",
		W2A(bstrCode), varTime.lVal, varExCur.fltVal, varExDiff.fltVal);
		
}


void CFutExt9027::OnDestroy()
{
	__super::OnDestroy();
	if (m_pFutExtCur)
	{
		if (m_bIsSB)
			m_pFutExtCur->Unsubscribe();
		DispEventUnadvise(m_pFutExtCur);
	}

}

