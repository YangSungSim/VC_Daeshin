// MarketEyeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlusSamples.h"
#include "MarketEyeDlg.h"
#include "atlsafe.h"
#include <string>


// CMarketEyeDlg dialog

IMPLEMENT_DYNAMIC(CMarketEyeDlg, CDialog)

CMarketEyeDlg::CMarketEyeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarketEyeDlg::IDD, pParent)
{

}

CMarketEyeDlg::~CMarketEyeDlg()
{
}

void CMarketEyeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMarketEyeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMarketEyeDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMarketEyeDlg message handlers

void CMarketEyeDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	USES_CONVERSION;



	//IDibPtr pMarketEye = NULL;
	CString strErr = _T("��ſ���: ");


	//IDibPtr pMarketEye;
	CComPtr<CPSYSDIBLib::ISysDib> pMarketEye  = NULL;
	HRESULT hr;

	if( !pMarketEye )
	{
		hr = pMarketEye.CoCreateInstance(__uuidof(CPSYSDIBLib::MarketEye));
		if (FAILED(hr))
		{
			AfxMessageBox("(DSCBO1Lib::StockMst ���� ����");
			return;
		}
	}


	// �ʵ屸��
	{
		long fieldList[] = { 0, 4, 5, 6, 7, 10};	
		int nVTCnt = sizeof(fieldList) / sizeof(fieldList[0]);
		SAFEARRAY* psa = SafeArrayCreateVector(VT_I4, 0, nVTCnt);
		if(!psa)
			return;

		long *pElemField;
		hr = SafeArrayAccessData(psa, (void**)&pElemField);
		if(FAILED(hr))
			return;

		for(int i=0; i < nVTCnt; i++)
			pElemField[i] = fieldList[i];

		hr = SafeArrayUnaccessData(psa);
		if(FAILED(hr))
			return;

		VARIANT vtField;
		VariantInit(&vtField);
		vtField.parray = psa;
		vtField.vt = VT_I4|VT_ARRAY;
		pMarketEye->SetInputValue(0, vtField); 
		SafeArrayDestroy(psa);
	}


	//���񱸼�
	{
		CString codeList[] = { "A052790", "A078340", "A000660"}; 
		int nVTCnt = sizeof(codeList)/sizeof(codeList[0]);
		SAFEARRAY* psa = SafeArrayCreateVector(VT_BSTR, 0, nVTCnt);
		if(!psa)
			return;

		BSTR *pCodeElem;
		hr = SafeArrayAccessData(psa, (void**)&pCodeElem);
		if(FAILED(hr))
			return;

		for(int i=0; i < nVTCnt; i++)
			pCodeElem[i] = codeList[i].AllocSysString();

		hr = SafeArrayUnaccessData(psa);
		if(FAILED(hr))
			return;

		VARIANT vtCode;
		VariantInit(&vtCode);
		vtCode.parray = psa;
		vtCode.vt = VT_BSTR|VT_ARRAY;

		pMarketEye->SetInputValue(1, vtCode); 
		SafeArrayDestroy(psa);
	}



	// �� ��� ��û 
	hr = pMarketEye->BlockRequest();

	if (FAILED(hr))
	{
		AfxMessageBox("BlockRequest ����");
		return;
	}

	if (0 != pMarketEye->GetDibStatus() )
	{
		AfxMessageBox("��� ����");
		return;
	}


	// �� ����� ��ȸ 
	int nCntField = pMarketEye->GetHeaderValue(0);		// �ʵ尳��

	CString sTemp, strResultMsg;
	_variant_t vArray, vItem;
	CComSafeArray<BSTR> saFieldName;
	vArray = pMarketEye->GetHeaderValue(1);				// �ʵ��
	saFieldName.Attach(vArray.Detach().parray);

	LONG k,h;
	for(k=saFieldName.GetLowerBound(), h=saFieldName.GetUpperBound(); k <= h; k++)
	{
		vItem = saFieldName.GetAt(k);
		sTemp.Format("%s\t", (LPCTSTR)(bstr_t)vItem);
		strResultMsg += sTemp; 
	}
	strResultMsg += "\n\n";


	_variant_t vItem1, vItem2			;
	int nCntCode = pMarketEye->GetHeaderValue(2);		// ���񰳼�

	for(int i=0; i < nCntCode; i++)
	{
		for(int j=0; j < nCntField; j++)
		{
			vItem1 = pMarketEye->GetDataValue(j, i);
			vItem2.ChangeType(VT_BSTR, &vItem1);
			sTemp.Format("%s\t", (LPCTSTR)(bstr_t)vItem2);
			strResultMsg += sTemp;
		}

		strResultMsg += "\n";
	}

	AfxMessageBox(strResultMsg);

}
