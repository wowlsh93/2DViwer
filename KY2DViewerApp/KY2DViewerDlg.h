
// KY2DViewerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "KY2DEditor.h"

// CKY2DViewerDlg 대화 상자
class CKY2DViewerDlg
: public CDialog
{
// 생성입니다.
public:
	CKY2DViewerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KY2DVIEWERAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON				m_hIcon;
	CKY2DEditor			m_editor;
	CMFCColorButton		m_btn_color;
	// 생성된 메시지 맵 함수
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
