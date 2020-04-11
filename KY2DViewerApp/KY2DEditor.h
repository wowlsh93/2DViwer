#pragma once

#include "../KY2DViewer_DLL/KY2DViewer_IF.h"

// CKY2DEditor 대화 상자입니다.

class CKY2DEditor
: public CDialog
, public KY2DVIEWER::KY2DEDITEvents
{
	DECLARE_DYNAMIC(CKY2DEditor)

public:
	CKY2DEditor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CKY2DEditor();

	COLORREF	m_color;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KY2DEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	LONG config(LPCWSTR szCMD, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool On2DShapeSelect(KY2DVIEWER::KY2DSAHPE& shape);
	virtual bool On2DShapeDragStart(KY2DVIEWER::KY2DSAHPE& shape);
	virtual bool On2DShapeDragMove(KY2DVIEWER::KY2DSAHPE& shape);
	virtual bool On2DShapeDragEnd(KY2DVIEWER::KY2DSAHPE& shape);
	virtual bool On2DShapeCreate(KY2DVIEWER::KY2DSAHPE& shape);
};
