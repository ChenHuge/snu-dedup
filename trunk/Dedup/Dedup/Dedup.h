
// Dedup.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
////

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDedupApp:
// �� Ŭ������ ������ ���ؼ��� Dedup.cpp�� �����Ͻʽÿ�.
//

class CDedupApp : public CWinApp
{
public:
	CDedupApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDedupApp theApp;