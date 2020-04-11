
// KY2DViewerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KY2DViewerApp.h"
#include "KY2DViewerDlg.h"
#include "gnsoftech/gnutils.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKY2DViewerDlg ��ȭ ����
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


// CKY2DViewerDlg �޽��� ó����

BOOL CKY2DViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	
	m_editor.Create(m_editor.IDD, this);
	SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
	m_editor.SetFocus();

	return FALSE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CKY2DViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
