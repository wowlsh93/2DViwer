
// KY2DViewerApp.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "../KY2DViewer_DLL/KY2DViewer_IF.h"

// CKY2DViewerApp:
// �� Ŭ������ ������ ���ؼ��� KY2DViewerApp.cpp�� �����Ͻʽÿ�.
//

class CKY2DViewerApp : public CWinAppEx
{
public:
	CKY2DViewerApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CKY2DViewerApp theApp;