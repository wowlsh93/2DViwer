// KY2DViewer_DLL.h : KY2DViewer_DLL DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include <map>
#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "KY2DViewer.h"
#include "KY2DViewer_IF.h"

// CKY2DViewer_DLLApp
// �� Ŭ������ ������ ������ KY2DViewer_DLL.cpp�� �����Ͻʽÿ�.
//

class CKY2DViewer_DLLApp
: public CWinApp
{
protected:
	std::map<HWND, CKY2DViewer *>		m_map_viewers;

public:
	CKY2DViewer_DLLApp();

	CKY2DViewer* GetViewer(HWND hWnd);
	void PutViewer(HWND hWnd, CKY2DViewer* theViewer);
	void DelViewer(HWND hWnd);
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
