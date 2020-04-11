#pragma once

#define GLEW_STATIC
#include "gl/glew.h"
#include "gl/glm/glm.hpp"
#include "gl/glm/ext.hpp"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")

#define _USE_MATH_DEFINES
#include <math.h>

#include <list>
#include <algorithm>
#include <windows.h>
#include "gninput.hpp"
#include "gnthread.hpp"
#include "gnprofile.hpp"

class GNOpenGL;

class GNRenderItem
{
private:
	bool	m_init;
public:
	inline GNRenderItem() : m_init(true) {}
	inline virtual ~GNRenderItem() {}

	inline virtual bool OnStart(GNOpenGL& GNGL) { m_init = false; return true; }
	inline virtual bool OnUpdate(GNOpenGL& GNGL) { if (m_init) OnStart(GNGL); return true; }
	inline virtual bool OnBefore(GNOpenGL& GNGL) { return true; }
	inline virtual bool OnRender3D(GNOpenGL& GNGL) { return true; }
	inline virtual bool OnRender2D(GNOpenGL& GNGL) { return true; }
	inline virtual bool OnAfter(GNOpenGL& GNGL) { return true; }
	inline virtual LONG config_item(GNOpenGL& GNGL, const std::wstring& sCMD, WPARAM wParam, LPARAM lParam) { return 0; }
};

class GNOpenGL
: public GNThread
{
friend	class GNRenderItem;
protected:
	HWND		m_hWND;
	HDC			m_hDC;
	HGLRC		m_hRC;
	LONG		m_nFRAME;
	COLORREF	m_clearColor;
	CRect		m_rc;
	GNInput		m_inputs;

	glm::vec3	m_pos2D, m_rot2D;
	glm::vec3	m_pos3D, m_rot3D;

	GNSectionLocker				m_render_lock;
	std::list<GNRenderItem*>	m_render_items;

public:
	inline GNOpenGL(HWND hWND)
	: m_nFRAME(10000), m_hWND(hWND), m_clearColor(0x00FFFFFF)
	, m_rot2D(0.0f, 0.0f, 1.0f)
	, m_rot3D(0.0f, M_PI*0.5f, 10.0f)
	{
		m_hDC = ::GetDC(m_hWND);
		m_inputs.setHwnd(m_hWND);
		start();
	}

	inline virtual ~GNOpenGL()
	{
		stop();
		join();
		ReleaseDC(m_hWND, m_hDC);
	}
	
	inline void InitOpenGL()
	{
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),		// 픽셀포맷 설명자의 크기
			1,									// 버전
			PFD_DRAW_TO_WINDOW |				// 포맷이 반드시 윈도우를 지원해야 함
			PFD_SUPPORT_OPENGL |				// 포맷이 반드시 OpenGL을 지원해야 함
			PFD_DOUBLEBUFFER,					// 반드시 더블 버퍼링을 지원해야함
			PFD_TYPE_RGBA,						// RGBA 포맷을 요청
			24,									// 색상깊이를 선택
			0, 0, 0, 0, 0, 0,					// 색상비트 무시
			0,									// 알파버퍼 없음
			0,									// 쉬프트 비트 무시
			0,									// Accumulation 버퍼 없음
			0, 0, 0, 0,							// Accumulation 비트 무시
			24,									// 24비트 Z-버퍼 (깊이 버퍼)
			0,									// 스텐실 버퍼 없음
			0,									// Auxiliary 버퍼 없음
			PFD_MAIN_PLANE,						// 메인 드로잉 레이어
			0,									// 예약됨
			0, 0, 0								// 레이어 마스크 무시
		};
		int num_desc = DescribePixelFormat(m_hDC, 1, sizeof(pfd), &pfd);
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		int pixel_format = ChoosePixelFormat(m_hDC, &pfd);
		int ret = SetPixelFormat(m_hDC, pixel_format, &pfd);

		m_hRC = wglCreateContext(m_hDC);
		glewInit();

		wglMakeCurrent(m_hDC, m_hRC);
	}

	virtual void main()
	{
		InitOpenGL();

		static const double fpstime = 1.0 / 60.0;
		double ftime(0), etime(GNProfile::current()), ctime;
		while (!isStop())
		{
			if (!isStop())
			{
				GNAutoLocker locker(m_render_lock);

				Update();

				ctime = GNProfile::current();
				if (ctime - etime < fpstime)
				{
					BeforeRender();
					Render();
					AfterRender();
				}
			}
			while (!isStop() && (GNProfile::current() - etime < (fpstime*0.9)))
				::Sleep(1);
			etime += fpstime;
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	inline virtual bool Update()
	{
		++ m_nFRAME;
		GetClientRect(m_hWND, m_rc);
		m_inputs.update();

		for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin();
			cp != m_render_items.end(); cp ++)
				(*cp)->OnUpdate(*this);
		return true;
	}

	inline virtual bool SetPerspective()
	{
		glm::mat4 matProj = glm::perspective(30.0f, (float)m_rc.Width()/(float)m_rc.Height(), 0.1f, 1000.0f);
		glm::vec3 eye;
		eye.x = sinf((float)(m_rot3D.x + M_PI)) * cosf(m_rot3D.y) * m_rot3D.z;
		eye.z = cosf((float)(m_rot3D.x + M_PI)) * cosf(m_rot3D.y) * m_rot3D.z;
		eye.y = sinf(m_rot3D.y) * m_rot3D.z;
		eye += m_pos3D;

		glm::mat4 matView = glm::lookAt(eye, m_pos3D, glm::vec3(0,1,0));

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(matProj));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(matView));

		return true;
	}

	inline virtual bool SetOrtho()
	{
		glm::mat4 matProj = glm::ortho((float)m_rc.left, (float)m_rc.right, (float)m_rc.bottom, (float)m_rc.top, 5000.0f, -5000.0f);
		glm::mat4 matView = glm::lookAt(glm::vec3(0.0f, 0.0f, 5000.0f), glm::vec3(0.0f, 0.0f, -5000.0f), glm::vec3(0,1,0));

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(matProj));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(matView));

		glScalef(m_rot2D.z, m_rot2D.z, m_rot2D.z);
		glTranslatef(m_pos2D.x, m_pos2D.y, m_pos2D.z);
		return true;
	}
	
	inline virtual bool ScreenToClient(glm::vec2& pt)
	{
		pt.x = pt.x / m_rot2D.z - m_pos2D.x;
		pt.y = pt.y / m_rot2D.z - m_pos2D.y;
		return true;
	}

	inline virtual bool ClientToScreen(glm::vec2& pt)
	{
		pt.x = (pt.x + m_pos2D.x) * m_rot2D.z;
		pt.y = (pt.y + m_pos2D.y) * m_rot2D.z;
		return true;
	}

	inline virtual bool On3DCamera()
	{
		return true;
	}
	
	inline virtual bool On2DCamera()
	{
		return true;
	}
	
	inline virtual bool BeforeRender()
	{
		glViewport(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height());
		glClearColor(GetRValue(m_clearColor)/255.0f, GetGValue(m_clearColor)/255.0f, GetBValue(m_clearColor)/255.0f, (m_clearColor>>24)/255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin();
			cp != m_render_items.end(); cp ++)
				(*cp)->OnBefore(*this);
		return true;
	}
	
	inline virtual bool OnAfterRender()
	{
		return true;
	}

	inline virtual bool Render()
	{
		SetPerspective();
		On3DCamera();
		for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin();
			cp != m_render_items.end(); cp ++)
				(*cp)->OnRender3D(*this);

		SetOrtho();
		On2DCamera();
		for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin();
			cp != m_render_items.end(); cp ++)
				(*cp)->OnRender2D(*this);

		OnAfterRender();

		return true;
	}

	inline virtual bool AfterRender()
	{
		for (std::list<GNRenderItem*>::iterator cp = m_render_items.begin();
			cp != m_render_items.end(); cp ++)
				(*cp)->OnAfter(*this);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopAttrib();

		SwapBuffers(m_hDC);
		return true;
	}

	inline bool addRenderItem(GNRenderItem* obj)
	{
		GNAutoLocker locker(m_render_lock);
		std::list<GNRenderItem*>::iterator fp =
			std::find(m_render_items.begin(), m_render_items.end(), obj);
		if (fp == m_render_items.end())
			m_render_items.push_back(obj);
		return true;
	}

	inline bool removeRenderItem(GNRenderItem* obj)
	{
		GNAutoLocker locker(m_render_lock);
		std::list<GNRenderItem*>::iterator fp =
			std::find(m_render_items.begin(), m_render_items.end(), obj);
		if (fp != m_render_items.end())
		{
			m_render_items.erase(fp);
			return true;
		}
		return false;
	}
	
	inline virtual bool OnMsg(MSG& msg)
	{
		return true;
	}
};
