/*

 * This file was provided by Dr. Charles Owen at Michigan State University.

 */

/*

 * This class was given to me, but I modified it to use my source.

 * The ChildView class is the entry point to the graphics portion of this
 *   program, including the OnGLDraw that is used to render the scene.

 */

#ifndef __ChildView_h__
#define __ChildView_h__

#pragma once

#include <vector>
#include "OpenGLWnd.h"

// The eyepoint provides the user an interface to interact with the
//   program.  It is currently set up to use my eyepoint, but the given
//   eyepoint class can be used by commentting out the
//   #define __JASON_EYEPOINT__
#define __JASON_EYEPOINT__
#ifdef __JASON_EYEPOINT__
class Eyepoint;
#else
#include "grcamera.h"
#endif

class Model;
class CTexture;
class ModelLoader;
class Day;
class Night;
class Fountain;

class CChildView : public COpenGLWnd {
public:
  /*** Constructor/Destructor ***********************************************/
  CChildView();
  virtual ~CChildView();

  /*** Scene rendering method ***********************************************/
  void OnGLDraw(CDC *pDC);

protected:
  DECLARE_MESSAGE_MAP()

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  /*** Data *****************************************************************/
private:
  ModelLoader *d_modelLoader;

  // Models
  std::vector<Model *> d_models;
  Fountain *d_fountain;
  Model    *d_background;
  Day      *d_day;
  Night    *d_night;

  // Flow control information
  bool d_pause; // Pauses the workings of the world, but not the eyepoint
  bool d_first; // Used for initializing the program
  UINT_PTR d_timer; // Used to create the timer
  unsigned int d_toggle; // Changes the interface for the eyepoint

  // Eyepoint information
#ifdef __JASON_EYEPOINT__
  Eyepoint *d_eyepoint;
  long d_prevX;
  long d_prevY;
#else
  CGrCamera d_camera;
  double d_tsm;
  int d_deltax;
  int d_deltay;
#endif

  /*** My helper function for models ****************************************/
  void LoadModels(void);

public:
  /*** Generated callbacks **************************************************/
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnTimer(UINT nIDEvent);

};

#endif
