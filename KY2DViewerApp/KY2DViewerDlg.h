
// KY2DViewerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "KY2DEditor.h"

// CKY2DViewerDlg ��ȭ ����
class CKY2DViewerDlg
: public CDialog
{
// �����Դϴ�.
public:
	CKY2DViewerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_KY2DVIEWERAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON				m_hIcon;
	CKY2DEditor			m_editor;
	CMFCColorButton		m_btn_color;
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnClickedAddRect();
	afx_msg void OnBnClickedAddEllipse();
	afx_msg void OnBnClickedBtnColor();
};
