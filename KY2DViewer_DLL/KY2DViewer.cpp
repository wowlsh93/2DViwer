#include "StdAfx.h"
#include "KY2DViewer.h"

CKY2DViewer::CKY2DViewer(HWND _hWND)
: m_hWND(_hWND), GNOpenGL(_hWND)
{
	addRenderItem(this);
	addRenderItem(&CKY2DShapes::get());
	m_inputs.setHwnd(m_hWND);
}

CKY2DViewer::~CKY2DViewer(void)
{
}

LONG CKY2DViewer::config(const std::wstring& sCMD, WPARAM wParam, LPARAM lParam)
{
	if (sCMD==L"2d_msg")
	{
		MSG& msg = *((MSG*)lParam);
		OnMsg(msg);
		return 0;
	}
	else if( sCMD == L"set_bk_color")
	{
		m_clearColor = (COLORREF)wParam;
	}
	return config_items(sCMD, wParam, lParam);
}

LONG CKY2DViewer::config_items(const std::wstring& sCMD, WPARAM wParam, LPARAM lParam)
{
	GNAutoLocker locker(m_render_lock);
	for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin(); cp != m_render_items.end(); cp ++)
		(*cp)->config_item(*this, sCMD, wParam, lParam);
	return 0;
}

bool CKY2DViewer::OnMsg(MSG& msg)
{
	static CPoint ptLDown, ptRDown;
	static glm::vec3 s_rot, s_pos;
	static bool s_drag = false;

	switch (msg.message)
	{
		case WM_MOUSEWHEEL:
		{
			CPoint pt(GetMessagePos());
			::ScreenToClient(msg.hwnd, &pt);

			glm::vec2 po(pt.x, pt.y), px(po);
			ScreenToClient(po);

			short zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
			m_rot3D.z += (zDelta / 120.0f) * 0.1f;
			m_rot3D.z = glm::max(0.1f, m_rot3D.z);
			m_rot2D.z += (zDelta / 120.0f) * (m_rot2D.z * 0.1f);
			m_rot2D.z = glm::max(0.1f, m_rot2D.z);
			ScreenToClient(px);

			glm::vec2 dp = px - po;
			m_pos2D.x += dp.x;
			m_pos2D.y += dp.y;
		} break;
		
		case WM_LBUTTONDOWN:
		{
			if (::GetCapture() != m_hWND)
				SetCapture(m_hWND);
			CPoint pt(msg.lParam);
			ptLDown = pt;
			s_rot = m_rot3D;
			config_items(L"WM_LBUTTONDOWN", 0, msg.lParam);
		} break;

		case WM_LBUTTONUP:
		{
			config_items(L"WM_LBUTTONUP", 0, msg.lParam);
			CPoint pt(msg.lParam);

			if (m_inputs.isPress(VK_RBUTTON))
				break;
			if (::GetCapture() == m_hWND)
				ReleaseCapture();
		} break;
		
		case WM_RBUTTONDOWN:
		{
			if (::GetCapture() != m_hWND)
				SetCapture(m_hWND);
			CPoint pt(msg.lParam);
			ptRDown = pt;
			s_pos = m_pos2D;
			config_items(L"WM_RBUTTONDOWN", 0, msg.lParam);
		} break;

		case WM_RBUTTONUP:
		{
			config_items(L"WM_RBUTTONUP", 0, msg.lParam);

			if (m_inputs.isPress(VK_LBUTTON))
				break;
			if (::GetCapture() == m_hWND)
				ReleaseCapture();
		} break;

		case WM_MOUSEMOVE:
		{
			CPoint pt(msg.lParam);

			if (msg.wParam & MK_LBUTTON)
			{
				config_items(L"WM_MOUSEMOVE", msg.wParam, msg.lParam);

				CPoint dt = ptLDown - pt;
				m_rot3D.x = (float)(s_rot.x + dt.x / 300.0f * M_PI);
				m_rot3D.y = (float)(s_rot.y + dt.y / 300.0f * M_PI);
				m_rot3D.y = glm::min(m_rot3D.y, +89.99f);
				m_rot3D.y = glm::max(m_rot3D.y, -89.99f);
			}

			if (msg.wParam & MK_RBUTTON)
			{
				config_items(L"WM_MOUSEMOVE_R", msg.wParam, msg.lParam);

				CPoint dt = pt - ptRDown;
				m_pos2D = s_pos + glm::vec3(dt.x, dt.y, 0.0f) / m_rot2D.z;
			}
		} break;
	}

	return true;
}

bool CKY2DViewer::OnRender2D(GNOpenGL& GNGL)
{
#if 0
	glPushMatrix();
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 100.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();
#endif
	return GNRenderItem::OnRender2D(GNGL);
}

bool Vec2InVec4(const glm::vec4& v4, const glm::vec2& po)
{
	return (po.x >= v4.x && po.x <= v4.z && po.y >= v4.y && po.y <= v4.w);
}

LONG CKY2DShapes::config_item(GNOpenGL& GNGL, const std::wstring& sCMD, WPARAM wParam, LPARAM lParam)
{
	if (sCMD == L"clearShapes")
	{
		GNAutoLocker locker(m_lock_2dshape);
		m_map_2dshapes.clear();
		m_lst_2dshapes.clear();
		return 1;
	}
	else if (sCMD == L"addShapes")
	{
		GNAutoLocker locker(m_lock_2dshape);
		KY2DVIEWER::KY2DSAHPE *pShares = (KY2DVIEWER::KY2DSAHPE *)lParam;
		for (int i = 0; i < wParam; i++)
		{
			if (pShares[i].m_nID2DShape == -1)
				pShares[i].m_nID2DShape = NewShapeID();
			else
				s_shapeID() = std::max(s_shapeID(), pShares[i].m_nID2DShape);

			if (m_map_2dshapes.find(pShares[i].m_nID2DShape) == m_map_2dshapes.end())
			{
				m_map_2dshapes[pShares[i].m_nID2DShape] = pShares[i];
				KY2DVIEWER::KY2DSAHPE& shape = m_map_2dshapes[pShares[i].m_nID2DShape];
				m_lst_2dshapes.push_back(&shape);
			}
			else
			{
				m_map_2dshapes[pShares[i].m_nID2DShape] = pShares[i];
			}
		}
		return 1;
	}
	else if (sCMD == L"removeShape")
	{
		GNAutoLocker locker(m_lock_2dshape);
		std::map<int, KY2DVIEWER::KY2DSAHPE>::iterator fp = m_map_2dshapes.find((INT)wParam);
		if (fp != m_map_2dshapes.end())
		{
			std::list<KY2DVIEWER::KY2DSAHPE*>::iterator cp = std::find(m_lst_2dshapes.begin(), m_lst_2dshapes.end(), &(fp->second));
			if (cp != m_lst_2dshapes.end()) m_lst_2dshapes.erase(cp);
			m_map_2dshapes.erase(fp);
		}
		return 1;
	}
	else if (sCMD == L"addEventListener")
	{
		KY2DVIEWER::KY2DEDITEvents* eListener = (KY2DVIEWER::KY2DEDITEvents*)lParam;
		GNAutoLocker locker(m_lock_listener);
		std::list<KY2DVIEWER::KY2DEDITEvents*>::iterator fp = std::find(m_lst_listeners.begin(), m_lst_listeners.end(), eListener);
		if (fp == m_lst_listeners.end())
		{
			m_lst_listeners.push_back(eListener);
			return 0;
		}
		return 1;
	}
	else if (sCMD == L"removeEventListener")
	{
		KY2DVIEWER::KY2DEDITEvents* eListener = (KY2DVIEWER::KY2DEDITEvents*)lParam;
		GNAutoLocker locker(m_lock_listener);
		std::list<KY2DVIEWER::KY2DEDITEvents*>::iterator fp = std::find(m_lst_listeners.begin(), m_lst_listeners.end(), eListener);
		if (fp != m_lst_listeners.end())
		{
			m_lst_listeners.erase(fp);
			return 0;
		}
		return 1;
	}
	else if (sCMD == L"createShape")
	{
		m_create_2dshape = true;
		m_tracker_mode = 0;
		m_pick_2dshape = NULL;
		memset(&m_crt_2dshape, 0, sizeof(m_crt_2dshape));
		m_crt_2dshape.m_crColor = m_colorCreate;
		m_crt_2dshape.m_e2DShapeType = (INT)wParam;
		m_crt_2dshape.m_nID2DShape = -3;
	}
	else if (sCMD == L"selectShape")
	{
		m_create_2dshape = false;
		m_tracker_mode = 0;
		m_pick_2dshape = NULL;
		std::map<INT,KY2DVIEWER::KY2DSAHPE>::iterator fp = m_map_2dshapes.find((INT)wParam);
		if (fp == m_map_2dshapes.end())
			return 1;

		m_pick_2dshape = &fp->second;
		dispatchEvent(0, fp->second);
		return 0;
	}
	else if (sCMD == L"selectCreateColor")
	{
		m_colorCreate = (COLORREF)lParam;
		m_crt_2dshape.m_crColor = m_colorCreate;
	}
	else if (sCMD == L"changeShapeColor")
	{
		INT nID = *((INT*)&wParam);
		if (nID == -1)
		{
			if (m_pick_2dshape)
				m_pick_2dshape->m_crColor = (ULONG)lParam;
		}
		else
		{
			GNAutoLocker locker(m_lock_2dshape);
			if (m_map_2dshapes.find(nID) != m_map_2dshapes.end())
				m_map_2dshapes[nID].m_crColor = (ULONG)lParam;
		}
	}
	else if (sCMD == L"changeShapeType")
	{
		INT nID = *((INT*)&wParam);
		if (nID == -1)
		{
			if (m_pick_2dshape)
				m_pick_2dshape->m_e2DShapeType = (INT)lParam;
		}
		else
		{
			GNAutoLocker locker(m_lock_2dshape);
			if (m_map_2dshapes.find(nID) != m_map_2dshapes.end())
				m_map_2dshapes[nID].m_e2DShapeType = (INT)lParam;
		}
	}
	else if (sCMD == L"WM_LBUTTONDOWN")
	{
		CPoint pt(lParam);
		glm::vec2 po(pt.x, pt.y);
		GNGL.ScreenToClient(po);
		
		if (m_create_2dshape)
		{
			m_crt_2dshape.m_ptCenterX = po.x;
			m_crt_2dshape.m_ptCenterY = po.y;
			m_crt_2dshape.m_szSizeW = 0.0f;
			m_crt_2dshape.m_szSizeH = 0.0f;
			m_crt_2dshape.m_nID2DShape = -2;
		}
		else if (m_pick_2dshape)
		{
			glm::vec2 lt(m_pick_2dshape->m_ptCenterX-m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY-m_pick_2dshape->m_szSizeH*0.5f);
			glm::vec2 br(m_pick_2dshape->m_ptCenterX+m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY+m_pick_2dshape->m_szSizeH*0.5f);

			GNGL.ClientToScreen(lt);
			GNGL.ClientToScreen(br);

			glm::vec2 pi(pt.x, pt.y);
			m_tracker_mode = 0;
			if (Vec2InVec4(glm::vec4(lt.x-m_tracker_size*0.5f, lt.y-m_tracker_size*0.5f,lt.x+m_tracker_size*0.5f, lt.y+m_tracker_size*0.5f), pi))
				m_tracker_mode = 2;
			else if (Vec2InVec4(glm::vec4(br.x-m_tracker_size*0.5f, lt.y-m_tracker_size*0.5f,br.x+m_tracker_size*0.5f, lt.y+m_tracker_size*0.5f), pi))
				m_tracker_mode = 4;
			else if (Vec2InVec4(glm::vec4(br.x-m_tracker_size*0.5f, br.y-m_tracker_size*0.5f,br.x+m_tracker_size*0.5f, br.y+m_tracker_size*0.5f), pi))
				m_tracker_mode = 6;
			else if (Vec2InVec4(glm::vec4(lt.x-m_tracker_size*0.5f, br.y-m_tracker_size*0.5f,lt.x+m_tracker_size*0.5f, br.y+m_tracker_size*0.5f), pi))
				m_tracker_mode = 8;
			else
			{
				bool r_f = true;
				float r = 0.0f;
				KY2DVIEWER::KY2DSAHPE* oShape = NULL;
				GNAutoLocker locker(m_lock_2dshape);
				if (! m_lst_2dshapes.empty())
				{
					for (std::list<KY2DVIEWER::KY2DSAHPE*>::iterator cp = m_lst_2dshapes.begin(); cp != m_lst_2dshapes.end(); cp ++)
					{
						KY2DVIEWER::KY2DSAHPE* pShape = *cp;
						glm::vec4 v4(pShape->m_ptCenterX-pShape->m_szSizeW*0.5f, pShape->m_ptCenterY-pShape->m_szSizeH*0.5f
							, pShape->m_ptCenterX+pShape->m_szSizeW*0.5f, pShape->m_ptCenterY+pShape->m_szSizeH*0.5f);
						float rz = pShape->m_szSizeW * pShape->m_szSizeH;
						if (Vec2InVec4(v4, po) && (r_f || rz < r))
						{
							r = rz;
							oShape = pShape;
							r_f = false;
						}
					}
				}
				m_tracker_mode = (oShape && oShape == m_pick_2dshape) ? 1 : 0;
			}

			if (m_tracker_mode)
			{
				m_drag_pos = po;
				m_save_2dshape = *m_pick_2dshape;
				dispatchEvent(1, *m_pick_2dshape);
			}
		}
	}
	else if (sCMD == L"WM_MOUSEMOVE")
	{
		CPoint pi(lParam);
		glm::vec2 pt(pi.x, pi.y);
		GNGL.ScreenToClient(pt);

		if (m_create_2dshape)
		{
			glm::vec2 po(m_crt_2dshape.m_ptCenterX - m_crt_2dshape.m_szSizeW * 0.5f, m_crt_2dshape.m_ptCenterY - m_crt_2dshape.m_szSizeH * 0.5f);
			glm::vec2 dt(pt - po);
			m_crt_2dshape.m_szSizeW = sqrtf(dt.x * dt.x);
			m_crt_2dshape.m_szSizeH = sqrtf(dt.y * dt.y);
			m_crt_2dshape.m_ptCenterX = po.x + m_crt_2dshape.m_szSizeW *0.5f;
			m_crt_2dshape.m_ptCenterY = po.y + m_crt_2dshape.m_szSizeH *0.5f;
		}
		else if (m_tracker_mode)
		{
			glm::vec2 dt = pt - m_drag_pos;
			glm::vec2 lt(m_pick_2dshape->m_ptCenterX-m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY-m_pick_2dshape->m_szSizeH*0.5f);

			switch (m_tracker_mode)
			{
				case 1: // center
					m_pick_2dshape->m_ptCenterX = m_save_2dshape.m_ptCenterX + dt.x;
					m_pick_2dshape->m_ptCenterY = m_save_2dshape.m_ptCenterY + dt.y;
				break;
				case 2: // left top
					m_pick_2dshape->m_szSizeW = (m_save_2dshape.m_szSizeW - dt.x);
					m_pick_2dshape->m_szSizeH = (m_save_2dshape.m_szSizeH - dt.y);
					m_pick_2dshape->m_szSizeW = std::max(0.0f, m_pick_2dshape->m_szSizeW);
					m_pick_2dshape->m_szSizeH = std::max(0.0f, m_pick_2dshape->m_szSizeH);

					m_pick_2dshape->m_ptCenterX = (m_save_2dshape.m_ptCenterX + dt.x * 0.5f);
					m_pick_2dshape->m_ptCenterY = (m_save_2dshape.m_ptCenterY + dt.y * 0.5f);
				break;
				case 4: // right top
					m_pick_2dshape->m_szSizeW = (m_save_2dshape.m_szSizeW + dt.x);
					m_pick_2dshape->m_szSizeH = (m_save_2dshape.m_szSizeH - dt.y);
					m_pick_2dshape->m_szSizeW = std::max(0.0f, m_pick_2dshape->m_szSizeW);
					m_pick_2dshape->m_szSizeH = std::max(0.0f, m_pick_2dshape->m_szSizeH);

					m_pick_2dshape->m_ptCenterX = (m_save_2dshape.m_ptCenterX + dt.x * 0.5f);
					m_pick_2dshape->m_ptCenterY = (m_save_2dshape.m_ptCenterY + dt.y * 0.5f);
				break;
				case 6: // right bottom
					m_pick_2dshape->m_szSizeW = (m_save_2dshape.m_szSizeW + dt.x);
					m_pick_2dshape->m_szSizeH = (m_save_2dshape.m_szSizeH + dt.y);
					m_pick_2dshape->m_szSizeW = std::max(0.0f, m_pick_2dshape->m_szSizeW);
					m_pick_2dshape->m_szSizeH = std::max(0.0f, m_pick_2dshape->m_szSizeH);

					m_pick_2dshape->m_ptCenterX = (m_save_2dshape.m_ptCenterX + dt.x * 0.5f);
					m_pick_2dshape->m_ptCenterY = (m_save_2dshape.m_ptCenterY + dt.y * 0.5f);
				break;
				case 8: // left bottom
					m_pick_2dshape->m_szSizeW = (m_save_2dshape.m_szSizeW - dt.x);
					m_pick_2dshape->m_szSizeH = (m_save_2dshape.m_szSizeH + dt.y);
					m_pick_2dshape->m_szSizeW = std::max(0.0f, m_pick_2dshape->m_szSizeW);
					m_pick_2dshape->m_szSizeH = std::max(0.0f, m_pick_2dshape->m_szSizeH);

					m_pick_2dshape->m_ptCenterX = (m_save_2dshape.m_ptCenterX + dt.x * 0.5f);
					m_pick_2dshape->m_ptCenterY = (m_save_2dshape.m_ptCenterY + dt.y * 0.5f);
				break;
				default:
				return 1;
			}
			dispatchEvent(2, *m_pick_2dshape);
		}
	}
	else if (sCMD == L"WM_LBUTTONUP")
	{
		if (m_create_2dshape)
		{
			m_create_2dshape = false;
			if (dispatchEvent(4, m_crt_2dshape) && m_crt_2dshape.m_nID2DShape != -2)
			{
				GNAutoLocker locker(m_lock_2dshape);

				if (m_crt_2dshape.m_nID2DShape == -1)
					m_crt_2dshape.m_nID2DShape = NewShapeID();
				else
					s_shapeID() = std::max(s_shapeID(), m_crt_2dshape.m_nID2DShape);

				if (m_map_2dshapes.find(m_crt_2dshape.m_nID2DShape)==m_map_2dshapes.end())
				{
					m_map_2dshapes[m_crt_2dshape.m_nID2DShape] = m_crt_2dshape;
					m_pick_2dshape = &m_map_2dshapes[m_crt_2dshape.m_nID2DShape];
					m_lst_2dshapes.push_back(m_pick_2dshape);
				}
				else
				{
					m_map_2dshapes[m_crt_2dshape.m_nID2DShape] = m_crt_2dshape;
					m_pick_2dshape = &m_map_2dshapes[m_crt_2dshape.m_nID2DShape];
				}
				dispatchEvent(0, m_map_2dshapes[m_crt_2dshape.m_nID2DShape]);
			}
		}
		else if (m_tracker_mode)
		{
			m_tracker_mode = 0;
			if (m_pick_2dshape)
				dispatchEvent(3, *m_pick_2dshape);
		}
		else
		{
			CPoint pt(lParam);
			glm::vec2 po(pt.x, pt.y);
			GNGL.ScreenToClient(po);
			bool r_f = true;
			float r = 0.0f;

			GNAutoLocker locker(m_lock_2dshape);
			if (! m_lst_2dshapes.empty())
			{
				m_pick_2dshape = NULL;
				for (std::list<KY2DVIEWER::KY2DSAHPE*>::iterator cp = m_lst_2dshapes.begin(); cp != m_lst_2dshapes.end(); cp ++)
				{
					KY2DVIEWER::KY2DSAHPE* pShape = *cp;
					glm::vec4 v4(pShape->m_ptCenterX-pShape->m_szSizeW*0.5f, pShape->m_ptCenterY-pShape->m_szSizeH*0.5f
						, pShape->m_ptCenterX+pShape->m_szSizeW*0.5f, pShape->m_ptCenterY+pShape->m_szSizeH*0.5f);
					float rz = pShape->m_szSizeW * pShape->m_szSizeH;
					if (po.x >= v4.x && po.x <= v4.z && po.y >= v4.y && po.y <= v4.w && (r_f || rz < r))
					{
						r = rz;
						m_pick_2dshape = pShape;
						r_f = false;
					}
				}
				
				if (m_pick_2dshape)
					dispatchEvent(0, *m_pick_2dshape);
			}
		}
	}
	return 0;
}

void shape2Vertex(KY2DVIEWER::KY2DSAHPE& shape, std::vector<glm::vec3>& vPoints, std::vector<glm::vec4>& vColors)
{
	glm::vec4 color(glm::vec3(GetRValue(shape.m_crColor)/255.f,GetGValue(shape.m_crColor)/255.f,GetBValue(shape.m_crColor)/255.f), 1.0f);
	switch (shape.m_e2DShapeType)
	{
		case KY2DVIEWER::e2DShapeType_RECT:
		{
			Gdiplus::RectF rf;

			rf.X		= shape.m_ptCenterX;
			rf.Y		= shape.m_ptCenterY;
			rf.Width	= shape.m_szSizeW;
			rf.Height	= shape.m_szSizeH;
			rf.Offset(- rf.Width * 0.5f, - rf.Height * 0.5f);

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X				, rf.Y + rf.Height	, 1));
			
			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X				, rf.Y				, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X				, rf.Y				, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X + rf.Width		, rf.Y				, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X + rf.Width		, rf.Y				, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X + rf.Width		, rf.Y + rf.Height	, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X + rf.Width		, rf.Y + rf.Height	, 1));

			vColors.push_back(color);
			vPoints.push_back(glm::vec3(rf.X				, rf.Y + rf.Height	, 1));
		} break;

		case KY2DVIEWER::e2DShapeType_ELLIPSE:
		{
			glm::vec3 lPos, fPos;
			for (float r = 0.0f; r < 2.0f * M_PI; r += (float)((2.0f * M_PI)/24.0f))
			{
				glm::vec3 pos(sinf(r) * shape.m_szSizeW * 0.5f + shape.m_ptCenterX, cosf(r) * shape.m_szSizeH * 0.5f + shape.m_ptCenterY, 1);
				if (r == 0.0f)
				{
					fPos = pos;
				}
				else
				{
					vColors.push_back(color);
					vPoints.push_back(lPos);

					vColors.push_back(color);
					vPoints.push_back(pos);
				}
				lPos = pos;
			}
			vColors.push_back(color);
			vPoints.push_back(lPos);

			vColors.push_back(color);
			vPoints.push_back(fPos);
		} break;
		case KY2DVIEWER::e2DShapeType_POLYGON:
		break;
		case KY2DVIEWER::e2DShapeType_GRID:
		break;
		case KY2DVIEWER::e2DShapeType_LINE:
		break;
		case KY2DVIEWER::e2DShapeType_TEXT:
		break;
	}
}

bool CKY2DShapes::OnRender2D(GNOpenGL& GNGL)
{
	//glLineWidth(1.25f);
	//glPointSize(1.0f);
	static std::vector<glm::vec3>	vPoints(100000);
	static std::vector<glm::vec4>	vColors(100000);

	vPoints.clear();
	vColors.clear();
	for (std::list<KY2DVIEWER::KY2DSAHPE*>::iterator cp = m_lst_2dshapes.begin(); cp != m_lst_2dshapes.end(); cp ++)
	{
		KY2DVIEWER::KY2DSAHPE& shape = *(*cp);
		shape2Vertex(shape, vPoints, vColors);
	}

	if (m_create_2dshape && m_crt_2dshape.m_nID2DShape == -2)
	{
		shape2Vertex(m_crt_2dshape, vPoints, vColors);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, glm::value_ptr(vPoints.at(0)));
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, glm::value_ptr(vColors.at(0)));

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1.2f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, (GLsizei)vPoints.size());
	glDisable(GL_BLEND);

	return GNRenderItem::OnRender2D(GNGL);
}

bool CKY2DShapes::OnAfter(GNOpenGL& GNGL)
{
	if (m_pick_2dshape)
	{
		glPointSize(m_tracker_size);
		glBegin(GL_POINTS);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(m_pick_2dshape->m_ptCenterX-m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY-m_pick_2dshape->m_szSizeH*0.5f, 0.0f);
			glVertex3f(m_pick_2dshape->m_ptCenterX+m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY-m_pick_2dshape->m_szSizeH*0.5f, 0.0f);
			glVertex3f(m_pick_2dshape->m_ptCenterX+m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY+m_pick_2dshape->m_szSizeH*0.5f, 0.0f);
			glVertex3f(m_pick_2dshape->m_ptCenterX-m_pick_2dshape->m_szSizeW*0.5f, m_pick_2dshape->m_ptCenterY+m_pick_2dshape->m_szSizeH*0.5f, 0.0f);
		glEnd();
	}
	return GNRenderItem::OnAfter(GNGL);
}

bool CKY2DShapes::dispatchEvent(INT eType, KY2DVIEWER::KY2DSAHPE& shape)
{
	GNAutoLocker locker(m_lock_listener);
	for (std::list<KY2DVIEWER::KY2DEDITEvents*>::iterator cp = m_lst_listeners.begin(); cp != m_lst_listeners.end(); cp++)
	{
		KY2DVIEWER::KY2DEDITEvents* pListener = *cp;
		switch (eType)
		{
			case 0:
				pListener->On2DShapeSelect(shape);
			break;
			case 1:
				pListener->On2DShapeDragStart(shape);
			break;
			case 2:
				pListener->On2DShapeDragMove(shape);
			break;
			case 3:
				pListener->On2DShapeDragEnd(shape);
			break;
			case 4:
				pListener->On2DShapeCreate(shape);
			break;
			default:
			return false;
		}
	}
	return true;
}
