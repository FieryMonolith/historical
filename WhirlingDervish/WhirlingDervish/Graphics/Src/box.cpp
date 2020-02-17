#include <GL/gl.h>

#include "box.h"

Box::Box(void) {
  return;
}

Box::~Box(void) {
  return;
}

void Box::Draw(void) {
  glMatrixMode(GL_MODELVIEW);

  glTranslate3d(d_x, d_y, d_z);

  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x, d_y, d_z);
  glVertex3d(d_x + 20.0, d_y, d_z);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z);
  glVertex3d(d_x, d_y + 20.0, d_z);
  glEnd();

  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x, d_y, d_z+20.0);
  glVertex3d(d_x + 20.0, d_y, d_z+20.0);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z+20.0);
  glVertex3d(d_x, d_y + 20.0, d_z+20.0);
  glEnd();

  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x, d_y, d_z);
  glVertex3d(d_x, d_y, d_z + 20.0);
  glVertex3d(d_x, d_y + 20.0, d_z + 20.0);
  glVertex3d(d_x, d_y + 20.0, d_z);
  glEnd();

  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x + 20.0, d_y, d_z);
  glVertex3d(d_x + 20.0, d_y, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z);
  glEnd();

  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x, d_y, d_z);
  glVertex3d(d_x, d_y, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y, d_z);
  glEnd();

  glColor3f(0.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3d(d_x, d_y + 20.0, d_z);
  glVertex3d(d_x, d_y + 20.0, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z + 20.0);
  glVertex3d(d_x + 20.0, d_y + 20.0, d_z);
  glEnd();

  return;
}
