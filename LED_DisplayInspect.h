
// LED_DisplayInspect.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CLED_DisplayInspectApp:
// �� Ŭ������ ������ ���ؼ��� LED_DisplayInspect.cpp�� �����Ͻʽÿ�.
//

class CLED_DisplayInspectApp : public CWinApp
{
public:
	CLED_DisplayInspectApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CLED_DisplayInspectApp theApp;