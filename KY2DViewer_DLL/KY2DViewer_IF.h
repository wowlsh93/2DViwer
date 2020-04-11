// KY2DViewer_IF.h : KY2DViewer_DLL 의 인터페이스 파일입니다.
//
#pragma once

#include <string>

#ifdef _EXPORTS
#define	KY2DAPI	__declspec(dllexport)
#else
#define	KY2DAPI	__declspec(dllimport)
#endif

namespace KY2DVIEWER
{
enum e2DShapeType
{
	e2DShapeType_RECT		= 0
,	e2DShapeType_ELLIPSE
,	e2DShapeType_POLYGON
,	e2DShapeType_GRID
,	e2DShapeType_LINE
,	e2DShapeType_TEXT
,	e2DShapeType_MAX		= 0xFFFFFFFF
};

struct KY2DSAHPE
{
	INT					m_nID2DShape;
	INT					m_e2DShapeType;
	float				m_ptCenterX;
	float				m_ptCenterY;
	float				m_szSizeW;
	float				m_szSizeH;
	float				m_fAngle;
	ULONG				m_crColor;
	INT					m_nCW;
	UINT				m_nFlags;
	std::wstring		m_sText;
	INT					m_nPointCount;
	void *				m_points;

	inline KY2DSAHPE() : m_nID2DShape(-1), m_e2DShapeType(e2DShapeType_RECT), m_fAngle(0.0f)
	, m_crColor(RGB(255, 255, 255)), m_ptCenterX(0), m_ptCenterY(0), m_szSizeW(0), m_szSizeH(0)
	, m_nCW(0), m_nPointCount(0), m_points(0), m_nFlags(0) {}
};

class KY2DEDITEvents
{
public:
	virtual bool On2DShapeSelect(KY2DVIEWER::KY2DSAHPE& shape) = 0;
	virtual bool On2DShapeDragStart(KY2DVIEWER::KY2DSAHPE& shape) = 0;
	virtual bool On2DShapeDragMove(KY2DVIEWER::KY2DSAHPE& shape) = 0;
	virtual bool On2DShapeDragEnd(KY2DVIEWER::KY2DSAHPE& shape) = 0;
	virtual bool On2DShapeCreate(KY2DVIEWER::KY2DSAHPE& shape) = 0;
};

extern "C" KY2DAPI BOOL PASCAL  viewer_create(HWND hWND);
extern "C" KY2DAPI BOOL PASCAL  viewer_delete(HWND hWND);
extern "C" KY2DAPI LONG PASCAL  viewer_config(HWND hWND, LPCWSTR szCMD, WPARAM wParam, LPARAM lParam);
};

/****************************************************************************************************
config commands
=====================================================================================================
배경색을 지정함
@param szCMD	"set_bk_color"
@param wParam	COLORREF 색상
@param lParam	0
-----------------------------------------------------------------------------------------------------
2DShape Event Listener를 등록
@param szCMD	"addEventListener"
@param wParam	0
@param lParam	KY2DEDITEvents*		등록 할 Event Listener 포인터
-----------------------------------------------------------------------------------------------------
2DShape Event Listener를 제거
@param szCMD	"removeEventListener"
@param wParam	0
@param lParam	KY2DEDITEvents*		제거 할 Event Listener 포인터
-----------------------------------------------------------------------------------------------------
2D Shape을 모두 제거
@param szCMD	"clearShapes"
@param wParam	0
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape 생성
@param szCMD	"createShape"
@param wParam	생성할 2DShape의 종류
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape을 등록
@param szCMD	"addShapes"
@param wParam	KY2DSAHPE 배열의 갯수
@param lParam	KY2DSHAPE 배열의 포인터
-----------------------------------------------------------------------------------------------------
2D Shape을 제거
@param szCMD	"removeShape"
@param wParam	제거할 2DShape의 아이디
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape 을 선택
@param szCMD	"selectShape"
@param wParam	선택할 2DShape의 아이디
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape 종류를 변경
@param szCMD	"changeShapeType"
@param wParam	선택할 2DShape의 아이디 또는 -1(현재 선택된 2DShape)
@param lParam	변경 할 2DShape 종류
-----------------------------------------------------------------------------------------------------
2D Shape 의 색상을 변경
@param szCMD	"changeShapeColor"
@param wParam	선택할 2DShape의 아이디 또는 -1(현재 선택된 2DShape)
@param lParam	COLORREF 변경 할 색상
-----------------------------------------------------------------------------------------------------
2D Shape 생성
@param szCMD	"selectCreateColor"
@param wParam	0
@param lParam	COLORREF 변경 할 색상
****************************************************************************************************/