#pragma once

#include <map>
#include <list>
#include <string>
#include <gdiplus.h>
#include "gnsoftech/gnopengl.hpp"
#include "gnsoftech/gninput.hpp"
#include "KY2DViewer_IF.h"

class CKY2DViewer
: public GNOpenGL
, public GNRenderItem
{
protected:
	HWND	m_hWND;

public:
	CKY2DViewer(HWND _hWND);
	virtual ~CKY2DViewer(void);

	LONG config(const std::wstring& sCMD, WPARAM wParam, LPARAM lParam);
	LONG config_items(const std::wstring& sCMD, WPARAM wParam, LPARAM lParam);
	GNInput		m_inputs;

protected:
	virtual bool OnMsg(MSG& msg);

	virtual bool OnRender2D(GNOpenGL& GNGL);
};

class CKY2DShapes
: public GNRenderItem
{
public:
	STATIC_INSTANCE(CKY2DShapes)
public:
	inline CKY2DShapes(): m_pick_2dshape(NULL), m_create_2dshape(false)
		, m_tracker_size(10.0f), m_tracker_mode(0), m_colorCreate(0L) {}
	inline virtual ~CKY2DShapes() {}

protected:
	GNSectionLocker							m_lock_2dshape;
	std::list<KY2DVIEWER::KY2DSAHPE*>		m_lst_2dshapes;
	std::map<int, KY2DVIEWER::KY2DSAHPE>	m_map_2dshapes;

	GNSectionLocker							m_lock_listener;
	std::list<KY2DVIEWER::KY2DEDITEvents*>	m_lst_listeners;

	KY2DVIEWER::KY2DSAHPE*					m_pick_2dshape;
	KY2DVIEWER::KY2DSAHPE					m_save_2dshape;
	KY2DVIEWER::KY2DSAHPE					m_crt_2dshape;

	glm::vec2		m_drag_pos;
	bool			m_create_2dshape;
	float			m_tracker_size;
	int				m_tracker_mode;
	COLORREF		m_colorCreate;

	virtual LONG config_item(GNOpenGL& GNGL, const std::wstring& sCMD, WPARAM wParam, LPARAM lParam);
	virtual bool OnRender2D(GNOpenGL& GNGL);
	virtual bool OnAfter(GNOpenGL& GNGL);
	
	bool dispatchEvent(INT eType, KY2DVIEWER::KY2DSAHPE& shape);
	
	inline static INT& s_shapeID() { static INT s_sID = 0; return s_sID; }
	inline INT NewShapeID()
	{
		INT nID = ++ s_shapeID();
		return nID;
	}
};
