// KY2DViewer_DLL.h : KY2DViewer_DLL DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include <map>
#include "resource.h"		// 주 기호입니다.
#include "KY2DViewer.h"
#include "KY2DViewer_IF.h"

// CKY2DViewer_DLLApp
// 이 클래스의 구현을 보려면 KY2DViewer_DLL.cpp를 참조하십시오.
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
// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
