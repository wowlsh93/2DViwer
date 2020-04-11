// KY2DEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KY2DViewerApp.h"
#include "KY2DEditor.h"
#include <vector>
#include "gnsoftech/gnutils.hpp"

// CKY2DEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CKY2DEditor, CDialog)

CKY2DEditor::CKY2DEditor(CWnd* pParent /*=NULL*/)
: CDialog(CKY2DEditor::IDD, pParent)
, m_color(0L)
{

}

CKY2DEditor::~CKY2DEditor()
{
}

void CKY2DEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKY2DEditor, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CKY2DEditor 메시지 처리기입니다.

BOOL CKY2DEditor::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_MOUSEACTIVATE)
		SetFocus();

	MSG msg;
	msg.hwnd = GetSafeHwnd();
	msg.message = message;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.time = GetTickCount();
	GetCursorPos(&msg.pt);
	KY2DVIEWER::viewer_config(GetSafeHwnd(), L"2d_msg", 0, (LPARAM)&msg);

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CKY2DEditor::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CKY2DEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	HWND m_h2DView = GetSafeHwnd();

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	KY2DVIEWER::viewer_create(m_h2DView);
	
	KY2DVIEWER::KY2DEDITEvents& eListener = *this;

	KY2DVIEWER::viewer_config(m_h2DView, L"addEventListener", 0, (LPARAM)&eListener);

	KY2DVIEWER::viewer_config(m_h2DView, L"set_bk_color", RGB(60, 200, 60), 0);

	KY2DVIEWER::viewer_config(m_h2DView, L"clearShapes", 0, 0);

	std::vector<KY2DVIEWER::KY2DSAHPE> _shapes;
	_shapes.reserve(16);

	int nID = 100;
	{
		KY2DVIEWER::KY2DSAHPE shape;
		shape.m_nID2DShape = nID ++;
		shape.m_e2DShapeType = KY2DVIEWER::e2DShapeType_RECT;
		shape.m_ptCenterX = 100.0f;
		shape.m_ptCenterY = 100.0f;
		shape.m_szSizeW = 100.0f;
		shape.m_szSizeH = 100.0f;
		shape.m_crColor = RGB(170, 0, 0);
		shape.m_fAngle = 0.0f;
		_shapes.push_back(shape);
	}

	{
		KY2DVIEWER::KY2DSAHPE shape;
		shape.m_nID2DShape = nID ++;
		shape.m_e2DShapeType = KY2DVIEWER::e2DShapeType_ELLIPSE;
		shape.m_ptCenterX =  70.0f;
		shape.m_ptCenterY = 130.0f;
		shape.m_szSizeW = 20.0f;
		shape.m_szSizeH = 20.0f;
		shape.m_crColor = RGB(0, 120, 120);
		shape.m_fAngle = 45.0f;
		_shapes.push_back(shape);
	}

	for( int i = 0; i < 5; i++)
	{
		KY2DVIEWER::KY2DSAHPE shape;
		shape.m_nID2DShape = nID ++;
		shape.m_e2DShapeType = KY2DVIEWER::e2DShapeType_RECT;
		shape.m_ptCenterX =  60.0f + i *20;
		shape.m_ptCenterY =  160.0f;
		shape.m_szSizeW = 10.0f;
		shape.m_szSizeH = 20.0f;
		shape.m_crColor = RGB(170, 0, 0);
		shape.m_fAngle = 0.0f;
		_shapes.push_back(shape);
	}

	for( int i = 0; i < 5; i++)
	{
		KY2DVIEWER::KY2DSAHPE shape;
		shape.m_nID2DShape = nID ++;
		shape.m_e2DShapeType = KY2DVIEWER::e2DShapeType_RECT;
		shape.m_ptCenterX =  160.0f;
		shape.m_ptCenterY =  140.0f - i *20;
		shape.m_szSizeW = 20.0f;
		shape.m_szSizeH = 10.0f;
		shape.m_crColor = RGB(170, 0, 0);
		shape.m_fAngle = 0.0f;
		_shapes.push_back(shape);
	}

	KY2DVIEWER::viewer_config(m_h2DView, L"addShapes", _shapes.size(), (LPARAM)&(_shapes.at(0)));

	return TRUE;
}

BOOL CKY2DEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == BN_CLICKED)
		if (nID == IDOK || nID == IDCANCEL)
			return FALSE;
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LONG CKY2DEditor::config(LPCWSTR szCMD, WPARAM wParam, LPARAM lParam)
{
	return KY2DVIEWER::viewer_config(GetSafeHwnd(), szCMD, wParam, lParam);
}

bool CKY2DEditor::On2DShapeSelect(KY2DVIEWER::KY2DSAHPE& shape)
{
	return false;
}

bool CKY2DEditor::On2DShapeDragStart(KY2DVIEWER::KY2DSAHPE& shape)
{
	return false;
}

bool CKY2DEditor::On2DShapeDragMove(KY2DVIEWER::KY2DSAHPE& shape)
{
	return false;
}

bool CKY2DEditor::On2DShapeDragEnd(KY2DVIEWER::KY2DSAHPE& shape)
{
	return false;
}

bool CKY2DEditor::On2DShapeCreate(KY2DVIEWER::KY2DSAHPE& shape)
{
	shape.m_nID2DShape = -1;
	shape.m_crColor = m_color;
	return false;
}

