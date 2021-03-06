// CStockBidDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "PlusSamples.h"
#include "StockBidDlg.h"
#include "afxdialogex.h"
#include "atlsafe.h"
#include <string>
#include <vector>
using namespace std;

struct STOCK_BID_ITEM
{
	long time;
	long cur;
	long diff;
	long offer;
	long bid;
};

// CStockBidDlg 대화 상자

IMPLEMENT_DYNAMIC(CStockBidDlg, CDialogEx)

CStockBidDlg::CStockBidDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STOCK_BID, pParent)
	, m_sCode(_T(""))
{

}

CStockBidDlg::~CStockBidDlg()
{
}

void CStockBidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_CODE, m_sCode);
	DDV_MaxChars(pDX, m_sCode, 7);

}


BEGIN_MESSAGE_MAP(CStockBidDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CStockBidDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStockBidDlg 메시지 처리기


void CStockBidDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	USES_CONVERSION;



	//IDibPtr pStockBid = NULL;
	CString strErr = _T("통신에러: ");

	// 종목코드 정상 확인
	CString strCode = m_sCode;
	if (strCode == "")
		strCode = "A003540";
	strCode.MakeUpper();	// 대문자로 만들기 
	if (strCode[0] != 'A')
		strCode = "A" + strCode;


	//IDibPtr pStockBid;
	CComPtr<DSCBO1Lib::IDib> pStockBid = NULL;
	HRESULT hr;

	// 통신 객체 생성 
	if (!pStockBid)
	{
		hr = pStockBid.CoCreateInstance(__uuidof(DSCBO1Lib::StockBid));
		if (FAILED(hr))
		{
			AfxMessageBox("(DSCBO1Lib::StockBid 생성 실패");
			return;
		}
	}


	//CComBSTR bstInput = ;
	CComVariant varInput = strCode;
	pStockBid->SetInputValue(0, varInput);
	pStockBid->SetInputValue(2, 80); // 요청 개수, 최대 80개 
	pStockBid->SetInputValue(3, 'c'); // 체결가, 호가 비교방식 선택


	long lTotCnt = 0;
	vector<STOCK_BID_ITEM> vtStockBid;
	while (True)
	{
		hr = pStockBid->BlockRequest();

		if (FAILED(hr))
		{
			AfxMessageBox("BlockRequest 실패");
			return;
		}

		if (0 != pStockBid->GetDibStatus())
		{
			AfxMessageBox("통신 오류");
			return;
		}



		int cntData = pStockBid->GetHeaderValue(2);		// 데이타 개수 

		CComVariant varCur, varDiff, varTime, varVol, varOffer, varBid;


		STOCK_BID_ITEM item;

		for (int i = 0; i < cntData; i++)
		{
			varCur = pStockBid->GetDataValue(4, i);
			varTime = pStockBid->GetDataValue(9, i);
			varDiff = pStockBid->GetDataValue(1, i);
			varOffer = pStockBid->GetDataValue(2, i);
			varBid = pStockBid->GetDataValue(3, i);


			item.bid = varBid.llVal;
			item.offer = varOffer.llVal;
			item.cur = varCur.llVal;
			item.diff = varDiff.llVal;
			item.time = varTime.llVal;

			vtStockBid.push_back(item);
		}
		
		lTotCnt += cntData;
		// 연속 통신 가능 여부 체크 
		long lCont;
		pStockBid->get_Continue(&lCont);
		if (lCont == 0)
		{
			TRACE("연속 데이터 없음\n");
			break;
		}
		TRACE("연속 데이터 있음\n");
		if (lTotCnt > 500)
			break;
	}
	
	vector<STOCK_BID_ITEM>::iterator it = vtStockBid.begin();
	for (; it != vtStockBid.end(); ++it)
	{
		TRACE("시간 %d 현재가 %d 대비 %d, 매도호가 %d 매수호가 %d\n", 
			it->time, it->cur, it->diff, it->offer, it->bid);
	}



	

}
