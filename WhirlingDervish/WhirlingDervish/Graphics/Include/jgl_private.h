#ifndef __JGL_PRIVATE_H__
#define __JGL_PRIVATE_H__

#include "eyepoint_implementation.h"
#include "grid_implementation.h"

class Vector3d;
class iPopulationStorage;

class JGLPrivate {
public:
  JGLPrivate(iPopulationStorage &);
  ~JGLPrivate(void);

  Eyepoint &GetEyepoint(void);
  Grid &GetGrid(void) { return (Grid &)d_grid; }

  void InitializeGL(void);
  void ResizeGL(int width, int height);
  void PaintGL(void) const;

  void NormalView(void);
  void DownView(void);

private:
  iPopulationStorage &d_ps;
  EyepointImplementation d_eyepoint;
  EyepointImplementation d_eyepointDown;
  GridImplementation d_grid;
  int d_whichView;

  int d_width, d_height;

  double d_nearPlaneDistance, d_farPlaneDistance;
  double d_scaleX, d_scaleY;
  int d_originX, d_originY;

  void DrawEyepoint(void) const;
  void DrawGrid(void) const;
  void DrawExternalOpengl(void) const;

  void ConvertWindowCoordinates(Vector3d &to, int x, int y, 
				const Vector3d &eyepointLocation,
				const Vector3d &out);
};

#endif
