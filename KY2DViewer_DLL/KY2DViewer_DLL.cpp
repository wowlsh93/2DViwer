// KY2DViewer_DLL.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "KY2DViewer_DLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKY2DViewer_DLLApp

BEGIN_MESSAGE_MAP(CKY2DViewer_DLLApp, CWinApp)
END_MESSAGE_MAP()

// CKY2DViewer_DLLApp 생성
CKY2DViewer_DLLApp::CKY2DViewer_DLLApp()
{
}

// 유일한 CKY2DViewer_DLLApp 개체입니다.

CKY2DViewer_DLLApp theApp;

// CKY2DViewer_DLLApp 초기화

BOOL CKY2DViewer_DLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CKY2DViewer* CKY2DViewer_DLLApp::GetViewer(HWND hWnd)
{
	std::map<HWND, CKY2DViewer *>::iterator fp = m_map_viewers.find(hWnd);
	if (fp == m_map_viewers.end())
		return NULL;
	return fp->second;
}

void CKY2DViewer_DLLApp::PutViewer(HWND hWnd, CKY2DViewer* theViewer)
{
	m_map_viewers[hWnd] = theViewer;
}

void CKY2DViewer_DLLApp::DelViewer(HWND hWnd)
{
	std::map<HWND, CKY2DViewer *>::iterator fp = m_map_viewers.find(hWnd);
	if (fp == m_map_viewers.end())
		return;
	m_map_viewers.erase(fp);
}

namespace KY2DVIEWER
{

BOOL PASCAL viewer_create(HWND hWND)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CKY2DViewer* theViewer = theApp.GetViewer(hWND);
	if (theViewer)
		return FALSE;
	theViewer = new CKY2DViewer(hWND);
	theApp.PutViewer(hWND, theViewer);
	return TRUE;
}

BOOL PASCAL viewer_delete(HWND hWND)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CKY2DViewer* theViewer = theApp.GetViewer(hWND);
	if (!theViewer)
		return FALSE;
	theApp.DelViewer(hWND);
	delete theViewer;
	return TRUE;
}

LONG PASCAL viewer_config(HWND hWND, LPCWSTR szCMD, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CKY2DViewer* theViewer = theApp.GetViewer(hWND);
	if (!theViewer)
		return FALSE;
	return theViewer->config(szCMD, wParam, lParam);
}

}
