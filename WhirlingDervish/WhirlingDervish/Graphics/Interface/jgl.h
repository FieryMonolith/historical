#ifndef __JGL_H__
#define __JGL_H__

class JGLPrivate;
class Eyepoint;
class Grid;
class iPopulationStorage;

class JGL {
public:
  JGL(iPopulationStorage &);
  ~JGL(void);

  Eyepoint &GetEyepoint(void);
  Grid &GetGrid(void);

  void NormalView(void);
  void DownView(void);

  void InitializeGL();
  void PaintGL();
  void ResizeGL(int width, int height);

private:
  JGLPrivate *d_data;
};

#endif
