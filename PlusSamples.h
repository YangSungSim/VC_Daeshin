
// PlusSamples.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "PlusSamples_i.h"


// CPlusSamplesApp:
// �� Ŭ������ ������ ���ؼ��� PlusSamples.cpp�� �����Ͻʽÿ�.
//

class CPlusSamplesApp : public CWinAppEx
{
public:
	CPlusSamplesApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance(void);
};

extern CPlusSamplesApp theApp;