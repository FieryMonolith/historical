#ifndef __QT_IJGL_H__
#define __QT_IJGL_H__

#include <qgl.h>
#include "jgl.h"

class iPopulationStorage;

class iJGL : public QGLWidget {
  Q_OBJECT

public:
  iJGL(iPopulationStorage &, QWidget * = 0, const char * = 0);
  ~iJGL(void);

  QSizePolicy sizePolicy(void) const;

public slots:
  void IncreaseRotationSpeed(void);
  void DecreaseRotationSpeed(void);
  void IncreaseForwardSpeed(void);
  void DecreaseForwardSpeed(void);

  void MoveForward(void);
  void MoveBackward(void);
  void MoveLeft(void);
  void MoveRight(void);
  void MoveUp(void);
  void MoveDown(void);
  void YawLeft(void);
  void YawRight(void);
  void PitchUp(void);
  void PitchDown(void);

  void ToggleGrid(void);
  void ToggleView(void);

  void NormalView(void);
  void DownView(void);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

#if 0
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
#endif

private:
  JGL d_jgl;
};

#if 0
  int mousePressEvent(int width, int height, int x, int y, int s);
  void mouseReleaseEvent(void);
  void mouseMoveEvent(int x, int y);
  bool d_pressed;
  int d_typePressed;
  int64 d_typeId;
  double d_linePercent;
  bool d_frozen1, d_frozen2;
#endif

#endif
