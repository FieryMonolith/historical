/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#pragma once

#include <vector>

#include "OpenGLWnd.h"

#include "grcamera.h"

class DeathSphere;
class Snake;

/*

 * CChildView - This class is a MFC class that handles the keyboard, mouse,
 *   timer, and window messaging.  It is also responsible for controlling the
 *   scene and drawing the graphics.

 */
class CChildView : public COpenGLWnd {
// Construction
public:
  CChildView();
  virtual ~CChildView();

  // Draw the scene
  void OnGLDraw(CDC *pDC);

protected:
  DECLARE_MESSAGE_MAP()

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
  CGrCamera d_camera;
  // Used for camera motion
  int d_deltax;
  int d_deltay;

  UINT_PTR d_timer;
  bool d_first; // Used for initializing the graphics
  bool d_pause; // Pause the program dynamics
  int d_toggle; // Toggles between PITCHYAW mode and PANTILT mode

  DeathSphere *d_sphere1;
  DeathSphere *d_sphere2;
  DeathSphere *d_sphere3;
  DeathSphere *d_sphere4;
  DeathSphere *d_sphere5;
  DeathSphere *d_sphere6;
  DeathSphere *d_sphere7;
  GLUquadric *d_head; // Head of the target dummy

  Snake *d_snake;

public:
  // Left mouse button pauses
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  // If the right mouse button is down, then rotate the eyepoint
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  // Right mouse button allows one to rotate the eyepoint
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  // Handle keyboard
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  // The timer updates the positions all of the dynamic elements
  afx_msg void OnTimer(UINT nIDEvent);
};
