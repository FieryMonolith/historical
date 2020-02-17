// ChildView.h : interface of the CChildView class
//

#define MONTE_CARLO

#include "OpenGLWnd.h"
#include "grcamera.h"
#include "grobject.h"

#pragma once


// CChildView window

class CChildView : public COpenGLWnd
{
// Construction
public:
	CChildView();
	
// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	double m_size;

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnGLDraw(CDC *pDC);
	void Box(GLdouble, GLdouble, GLdouble, const GLdouble*);
	void Barbell();
	double m_spinangle;
	UINT m_spintimer;
	afx_msg void OnLabstuffSpin();
	afx_msg void OnTimer(UINT nIDEvent);
	void Tetrahedron(double height);
	void Tetra(GLdouble p_size, const GLdouble *p_color);
	void ConfigureRenderer(CGrRenderer *p_renderer);
	inline void Tri(GLdouble *v1, GLdouble *v2, GLdouble *v3)
	{
		glBegin(GL_POLYGON);
		glVertex3dv(v1);
		glVertex3dv(v2);
		glVertex3dv(v3);
		glEnd();
	}
	afx_msg void OnLabstuffTetrasize();
	int GetIterations() { return m_iterations; }
private:
	CGrCamera m_camera;
	CGrPtr<CGrTexture> m_spritetex;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCameraPitch();
	afx_msg void OnUpdateCameraPitch(CCmdUI *pCmdUI);
	afx_msg void OnCameraRoll();
	afx_msg void OnUpdateCameraRoll(CCmdUI *pCmdUI);
	afx_msg void OnCameraPan();
	afx_msg void OnUpdateCameraPan(CCmdUI *pCmdUI);
private:
	CGrPtr<CGrObject> m_scene;
	bool	m_raytrace;
	GLbyte  **m_rayimage;
	int     m_rayimagewidth;
	int     m_rayimageheight;
	int m_iterations;

public:
	afx_msg void OnRenderRaytrace();
	afx_msg void OnUpdateRenderRaytrace(CCmdUI *pCmdUI);
	afx_msg void OnOptionsAnti();
#ifdef MONTE_CARLO
	afx_msg void OnOptionsMonte();

  unsigned int m_mcRays;
#endif
};

