#ifdef VISUAL_STUDIO
#include <windows.h>
#endif
#include <cmath>
#include <GL/gl.h>

#include "grid_implementation.h"

GridImplementation::GridImplementation(
	   double originX, double originY, double originZ,
	   double relativeNegativeBoundX, double relativePositiveBoundX,
	   double relativeNegativeBoundY, double relativePositiveBoundY,
	   double relativeNegativeBoundZ, double relativePositiveBoundZ,
	   double xyTick, double xzTick, double yzTick) {
  //d_show = false;
  d_show = true;

  ChangeGrid(originX, originY, originZ,
	     relativeNegativeBoundX, relativePositiveBoundX,
	     relativeNegativeBoundY, relativePositiveBoundY,
	     relativeNegativeBoundZ, relativePositiveBoundZ,
	     xyTick, xzTick, yzTick);
  return;
}

GridImplementation::~GridImplementation(void) {
  return;
}

void GridImplementation::Show(void) {
  d_show = true;

  return;
}

void GridImplementation::Hide(void) {
  d_show = false;

  return;
}

void GridImplementation::ChangeGrid(
       double originX, double originY, double originZ,
       double relativeNegativeBoundX, double relativePositiveBoundX,
       double relativeNegativeBoundY, double relativePositiveBoundY,
       double relativeNegativeBoundZ, double relativePositiveBoundZ,
       double xyTick, double xzTick, double yzTick) {
  d_origin[0] = originX;
  d_origin[1] = originY;
  d_origin[2] = originZ;
  d_relativeNegativeBound[0] = relativeNegativeBoundX;
  d_relativePositiveBound[0] = relativePositiveBoundX;
  d_relativeNegativeBound[1] = relativeNegativeBoundY;
  d_relativePositiveBound[1] = relativePositiveBoundY;
  d_relativeNegativeBound[2] = relativeNegativeBoundZ;
  d_relativePositiveBound[2] = relativePositiveBoundZ;
  d_tick[0] = xyTick;
  d_tick[1] = xzTick;
  d_tick[2] = yzTick;

  // 0=xy, 1=xz, 2=yz
  d_drawPlane[0] = d_drawPlane[1] = d_drawPlane[2] = 1;

  if(d_relativeNegativeBound[0] == d_relativePositiveBound[0]) {
    d_drawPlane[0] = 0;
    d_drawPlane[1] = 0;
  }
  if(d_relativeNegativeBound[1] == d_relativePositiveBound[1]) {
    d_drawPlane[0] = 0;
    d_drawPlane[2] = 0;
  }
  if(d_relativeNegativeBound[2] == d_relativePositiveBound[2]) {
    d_drawPlane[1] = 0;
    d_drawPlane[2] = 0;
  }

  d_ticks[0] = d_ticks[1] = d_ticks[2] = 1;

  fabs(d_tick[0]);
  if(d_tick[0] < 0.001) {
    d_ticks[0] = 0;
  }

  fabs(d_tick[1]);
  if(d_tick[1] < 0.001) {
    d_ticks[1] = 0;
  }

  fabs(d_tick[2]);
  if(d_tick[2] < 0.001) {
    d_ticks[2] = 0;
  }

  return;
}

void GridImplementation::Draw(void) const {
  if(!d_show) {
    return;
  }

  // 0=xy, 1=xz, 2=yz
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glTranslatef(d_origin[0], d_origin[1], d_origin[2]);
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  if(d_drawPlane[0]) {
    DrawPlane(d_ticks[0], 0, 1, 2);
  }
  glColor3f(0.0, 1.0, 0.0);
  if(d_drawPlane[1]) {
    DrawPlane(d_ticks[1], 0, 2, 1);
  }
  glColor3f(0.0, 0.0, 1.0);
  if(d_drawPlane[2]) {
    DrawPlane(d_ticks[2], 1, 2, 0);
  }
  glColor3f(1.0, 1.0, 1.0);
  glEnd();

  glPopMatrix();

  return;
}

void GridImplementation::DrawPlane(int hasTicks, int order1, int order2, 
				   int independentIndex) const {
  double vec[3];
  int order[2] = {order1, order2};
  int i;

  // Draw Axis lines
  vec[independentIndex] = (d_relativeNegativeBound[independentIndex] + 
			      d_relativePositiveBound[independentIndex]) / 2.0;

  for(i=0; i<2; i++) {
    vec[order[i]] = d_relativeNegativeBound[order[i]];
    vec[order[(i+1)%2]] = (d_relativeNegativeBound[order[(i+1)%2]] + 
			      d_relativePositiveBound[order[(i+1)%2]]) / 2.0;
    glVertex3d(vec[0], vec[1], vec[2]);
    vec[order[i]] = d_relativePositiveBound[order[i]];
    glVertex3d(vec[0], vec[1], vec[2]);
  }

  if(!hasTicks) {
    return;
  }

  for(i=0; i<2; i++) {
    double bound[2] = {d_relativeNegativeBound[order[(i+1)%2]],
		       d_relativePositiveBound[order[(i+1)%2]]};
    double axis = (d_relativeNegativeBound[order[i]] + 
		   d_relativePositiveBound[order[i]]) / 2.0;
    // Positive
    vec[order[i]] = axis;
    while(vec[order[i]] < d_relativePositiveBound[order[i]]) {
      vec[order[(i+1)%2]] = bound[0];
      glVertex3d(vec[0], vec[1], vec[2]);
      vec[order[(i+1)%2]] = bound[1];
      glVertex3d(vec[0], vec[1], vec[2]);
      vec[order[i]] += d_tick[order[i]];
    }
    // Draw bound
    vec[order[i]] = d_relativePositiveBound[order[i]];
    vec[order[(i+1)%2]] = bound[0];
    glVertex3d(vec[0], vec[1], vec[2]);
    vec[order[(i+1)%2]] = bound[1];
    glVertex3d(vec[0], vec[1], vec[2]);

    // Positive
    vec[order[i]] = axis;
    while(vec[order[i]] > d_relativeNegativeBound[order[i]]) {
      vec[order[(i+1)%2]] = bound[0];
      glVertex3d(vec[0], vec[1], vec[2]);
      vec[order[(i+1)%2]] = bound[1];
      glVertex3d(vec[0], vec[1], vec[2]);
      vec[order[i]] -= d_tick[order[i]];
    }
    // Draw bound
    vec[order[i]] = d_relativeNegativeBound[order[i]];
    vec[order[(i+1)%2]] = bound[0];
    glVertex3d(vec[0], vec[1], vec[2]);
    vec[order[(i+1)%2]] = bound[1];
    glVertex3d(vec[0], vec[1], vec[2]);
  }

  return;
}
