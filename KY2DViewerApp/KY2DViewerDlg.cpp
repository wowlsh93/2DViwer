
// KY2DViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "KY2DViewerApp.h"
#include "KY2DViewerDlg.h"
#include "gnsoftech/gnutils.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKY2DViewerDlg 대화 상자
CKY2DViewerDlg::CKY2DViewerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CKY2DViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKY2DViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COLOR, m_btn_color);
}

BEGIN_MESSAGE_MAP(CKY2DViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADD_RECT, &CKY2DViewerDlg::OnBnClickedAddRect)
	ON_BN_CLICKED(IDC_ADD_ELLIPSE, &CKY2DViewerDlg::OnBnClickedAddEllipse)
	ON_BN_CLICKED(IDC_BTN_COLOR, &CKY2DViewerDlg::OnBnClickedBtnColor)
END_MESSAGE_MAP()


// CKY2DViewerDlg 메시지 처리기

BOOL CKY2DViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	m_editor.Create(m_editor.IDD, this);
	SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
	m_editor.SetFocus();

	return FALSE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKY2DViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKY2DViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CKY2DViewerDlg::DestroyWindow()
{
	KY2DVIEWER::viewer_delete(GetSafeHwnd());
	return CDialog::DestroyWindow();
}

BOOL CKY2DViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CKY2DViewerDlg::OnSize(UINT nType, int cx, int cy){
	CDialog::OnSize(nType, cx, cy);
	if (m_editor.GetSafeHwnd())
		m_editor.MoveWindow(0, 30, cx, cy-30);
}

BOOL CKY2DViewerDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CKY2DViewerDlg::OnBnClickedAddRect()
{
	m_editor.config(L"createShape", KY2DVIEWER::e2DShapeType_RECT, 0);
}

void CKY2DViewerDlg::OnBnClickedAddEllipse()
{
	m_editor.config(L"createShape", KY2DVIEWER::e2DShapeType_ELLIPSE, 0);
}

void CKY2DViewerDlg::OnBnClickedBtnColor()
{
	m_editor.m_color = m_btn_color.GetColor();
	m_editor.config(L"selectCreateColor", -1, m_editor.m_color);
	m_editor.config(L"changeShapeColor", -1, m_editor.m_color);
}
