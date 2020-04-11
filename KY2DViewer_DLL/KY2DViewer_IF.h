// KY2DViewer_IF.h : KY2DViewer_DLL �� �������̽� �����Դϴ�.
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
������ ������
@param szCMD	"set_bk_color"
@param wParam	COLORREF ����
@param lParam	0
-----------------------------------------------------------------------------------------------------
2DShape Event Listener�� ���
@param szCMD	"addEventListener"
@param wParam	0
@param lParam	KY2DEDITEvents*		��� �� Event Listener ������
-----------------------------------------------------------------------------------------------------
2DShape Event Listener�� ����
@param szCMD	"removeEventListener"
@param wParam	0
@param lParam	KY2DEDITEvents*		���� �� Event Listener ������
-----------------------------------------------------------------------------------------------------
2D Shape�� ��� ����
@param szCMD	"clearShapes"
@param wParam	0
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape ����
@param szCMD	"createShape"
@param wParam	������ 2DShape�� ����
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape�� ���
@param szCMD	"addShapes"
@param wParam	KY2DSAHPE �迭�� ����
@param lParam	KY2DSHAPE �迭�� ������
-----------------------------------------------------------------------------------------------------
2D Shape�� ����
@param szCMD	"removeShape"
@param wParam	������ 2DShape�� ���̵�
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape �� ����
@param szCMD	"selectShape"
@param wParam	������ 2DShape�� ���̵�
@param lParam	0
-----------------------------------------------------------------------------------------------------
2D Shape ������ ����
@param szCMD	"changeShapeType"
@param wParam	������ 2DShape�� ���̵� �Ǵ� -1(���� ���õ� 2DShape)
@param lParam	���� �� 2DShape ����
-----------------------------------------------------------------------------------------------------
2D Shape �� ������ ����
@param szCMD	"changeShapeColor"
@param wParam	������ 2DShape�� ���̵� �Ǵ� -1(���� ���õ� 2DShape)
@param lParam	COLORREF ���� �� ����
-----------------------------------------------------------------------------------------------------
2D Shape ����
@param szCMD	"selectCreateColor"
@param wParam	0
@param lParam	COLORREF ���� �� ����
****************************************************************************************************/