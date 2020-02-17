#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <cmath>
#include <cassert>

#include "Vector3d.h"

#include "ant.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ant::Ant(void) : d_pointSize(3.0), d_lineWidth(1.0), d_numberOfLegs(6),
                 d_neckLength(1.5), d_tailLength(4.0),
                 d_lowerArmLength(3.0), d_upperArmLength(2.5),
                 d_shoulderWidth(2.0),
                 d_shoulderSpacingLength(3.0),
                 d_legEnds(0), d_elbows(0), d_shoulders(0), d_spine(0),
                 d_head(0), d_tail(0), d_valid(false) {
  d_halfShoulderWidth = d_shoulderWidth / 2.0;

  d_legEnds = new Vector3d[d_numberOfLegs];
  assert(d_legEnds);
  d_elbows = new Vector3d[d_numberOfLegs];
  assert(d_elbows);
  d_shoulders = new Vector3d[d_numberOfLegs];
  assert(d_shoulders);
  d_spine = new Vector3d[d_numberOfLegs / 2];
  assert(d_spine);
  d_head = new Vector3d[1];
  assert(d_head);
  d_tail = new Vector3d[1];
  assert(d_tail);

  Pose(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  return;
}

Ant::~Ant(void) {
  if(d_legEnds != 0) {
    delete [] d_legEnds;
    d_legEnds = 0;
  }
  if(d_elbows != 0) {
    delete [] d_elbows;
    d_elbows = 0;
  }
  if(d_shoulders != 0) {
    delete [] d_shoulders;
    d_shoulders = 0;
  }
  if(d_spine != 0) {
    delete [] d_spine;
    d_spine = 0;
  }
  if(d_head != 0) {
    delete [] d_head;
    d_head = 0;
  }
  if(d_tail != 0) {
    delete [] d_tail;
    d_tail = 0;
  }

  return;
}

void Ant::Draw(void) const {
  if(d_valid == false) { return; }

  glPointSize(d_pointSize);
  glLineWidth(d_lineWidth);

  glColor3f(1.0, 0.0, 0.0);

  if(d_legEnds == 0 || d_elbows == 0 || d_shoulders == 0 ||
     d_spine == 0 || d_head == 0 || d_tail == 0) {
    return;
  }

  glBegin(GL_POINTS);
  for(unsigned int i=0; i<d_numberOfLegs; i++) {
    glVertex3dv(d_legEnds[i].Array());
    glVertex3dv(d_elbows[i].Array());
    glVertex3dv(d_shoulders[i].Array());
  }
  for(unsigned int i=0; i<d_numberOfLegs/2; i++) {
    glVertex3dv(d_spine[i].Array());
  }
  glVertex3dv(d_head->Array());
  glVertex3dv(d_tail->Array());
  glEnd();

  glBegin(GL_LINES);
  glVertex3dv(d_head->Array());
  glVertex3dv(d_spine[2].Array());

  glVertex3dv(d_shoulders[5].Array());
  glVertex3dv(d_shoulders[4].Array());
  glVertex3dv(d_shoulders[5].Array());
  glVertex3dv(d_elbows[5].Array());
  glVertex3dv(d_elbows[5].Array());
  glVertex3dv(d_legEnds[5].Array());
  glVertex3dv(d_shoulders[4].Array());
  glVertex3dv(d_elbows[4].Array());
  glVertex3dv(d_elbows[4].Array());
  glVertex3dv(d_legEnds[4].Array());

  glVertex3dv(d_spine[2].Array());
  glVertex3dv(d_spine[1].Array());

  glVertex3dv(d_shoulders[3].Array());
  glVertex3dv(d_shoulders[2].Array());
  glVertex3dv(d_shoulders[3].Array());
  glVertex3dv(d_elbows[3].Array());
  glVertex3dv(d_elbows[3].Array());
  glVertex3dv(d_legEnds[3].Array());
  glVertex3dv(d_shoulders[2].Array());
  glVertex3dv(d_elbows[2].Array());
  glVertex3dv(d_elbows[2].Array());
  glVertex3dv(d_legEnds[2].Array());

  glVertex3dv(d_spine[1].Array());
  glVertex3dv(d_spine[0].Array());

  glVertex3dv(d_shoulders[1].Array());
  glVertex3dv(d_shoulders[0].Array());
  glVertex3dv(d_shoulders[1].Array());
  glVertex3dv(d_elbows[1].Array());
  glVertex3dv(d_elbows[1].Array());
  glVertex3dv(d_legEnds[1].Array());
  glVertex3dv(d_shoulders[0].Array());
  glVertex3dv(d_elbows[0].Array());
  glVertex3dv(d_elbows[0].Array());
  glVertex3dv(d_legEnds[0].Array());

  glVertex3dv(d_spine[0].Array());
  glVertex3dv(d_tail->Array());
  glEnd();

  return;
}

void Ant::Pose(const Vector3d &frontRight,  const Vector3d &frontLeft, 
               const Vector3d &middleRight, const Vector3d &middleLeft, 
               const Vector3d &backRight,   const Vector3d &backLeft) {
  d_valid = false;
  int offGround = 0;
  int rightSide = 0;
  int leftSide  = 0;
  int front     = 0;
  int middle    = 0;
  int back      = 0;

  if(frontRight[1] > 0) {
    offGround++;
    rightSide++;
    front++;
  }
  d_legEnds[0] = frontRight;
  if(frontLeft[1] > 0) {
    offGround++;
    leftSide++;
    front++;
  }
  d_legEnds[1] = frontLeft;
  if(middleRight[1] > 0) {
    offGround++;
    rightSide++;
    middle++;
  }
  d_legEnds[2] = middleRight;
  if(middleLeft[1] > 0) {
    offGround++;
    leftSide++;
    middle++;
  }
  d_legEnds[3] = middleLeft;
  if(backRight[1] > 0) {
    offGround++;
    rightSide++;
    front++;
  }
  d_legEnds[4] = backRight;
  if(backLeft[1] > 0) {
    offGround++;
    leftSide++;
    back++;
  }
  d_legEnds[5] = backLeft;

  // Check for feet placement errors.  Only two feet are allowed off the
  // ground for a given pose.  Also, no two feet off the ground can be on 
  // the same side or in the same pair.
#if 0
  if(offGround > 2) {
    return;
  } else if(offGround == 2) {
    if(right == 2 || left == 2) {
      return;
    } else if(front == 2 || middle == 2 || back == 2) {
      return;
    }
  }
#endif

  Zero();


  d_tail[0][0] = 0.0; d_tail[0][1] = 0.0; d_tail[0][2] = 0.0;

  d_spine[0][0] = 0.0; d_spine[0][1] = 0.0; d_spine[0][2] = 3.0;
  d_spine[1][0] = 0.0; d_spine[1][1] = 0.0; d_spine[1][2] = 6.0;
  d_spine[2][0] = 0.0; d_spine[2][1] = 0.0; d_spine[2][2] = 9.0;

  d_head[0][0] = 0.0; d_head[0][1] = 0.0; d_head[0][2] = 12.0;

  d_shoulders[0][0] = -1.5; d_shoulders[0][1] = 0.0; d_shoulders[0][2] = 3.0;
  d_shoulders[1][0] =  1.5; d_shoulders[1][1] = 0.0; d_shoulders[1][2] = 3.0;
  d_shoulders[2][0] = -1.5; d_shoulders[2][1] = 0.0; d_shoulders[2][2] = 6.0;
  d_shoulders[3][0] =  1.5; d_shoulders[3][1] = 0.0; d_shoulders[3][2] = 6.0;
  d_shoulders[4][0] = -1.5; d_shoulders[4][1] = 0.0; d_shoulders[4][2] = 9.0;
  d_shoulders[5][0] =  1.5; d_shoulders[5][1] = 0.0; d_shoulders[5][2] = 9.0;

  d_elbows[0][0] = -4.5; d_elbows[0][1] = 0.0; d_elbows[0][2] = 3.0;
  d_elbows[1][0] =  4.5; d_elbows[1][1] = 0.0; d_elbows[1][2] = 3.0;
  d_elbows[2][0] = -4.5; d_elbows[2][1] = 0.0; d_elbows[2][2] = 6.0;
  d_elbows[3][0] =  4.5; d_elbows[3][1] = 0.0; d_elbows[3][2] = 6.0;
  d_elbows[4][0] = -4.5; d_elbows[4][1] = 0.0; d_elbows[4][2] = 9.0;
  d_elbows[5][0] =  4.5; d_elbows[5][1] = 0.0; d_elbows[5][2] = 9.0;

  d_legEnds[0][0] = -7.5; d_legEnds[0][1] = 0.0; d_legEnds[0][2] = 3.0;
  d_legEnds[1][0] =  7.5; d_legEnds[1][1] = 0.0; d_legEnds[1][2] = 3.0;
  d_legEnds[2][0] = -7.5; d_legEnds[2][1] = 0.0; d_legEnds[2][2] = 6.0;
  d_legEnds[3][0] =  7.5; d_legEnds[3][1] = 0.0; d_legEnds[3][2] = 6.0;
  d_legEnds[4][0] = -7.5; d_legEnds[4][1] = 0.0; d_legEnds[4][2] = 9.0;
  d_legEnds[5][0] =  7.5; d_legEnds[5][1] = 0.0; d_legEnds[5][2] = 9.0;

  return;
}

void Ant::Zero(void) {
  d_tail[0][0] = 0.0; d_tail[0][1] = 0.0; d_tail[0][2] = 0.0;

  d_spine[0][0] = 0.0; d_spine[0][1] = 0.0; d_spine[0][2] = 0.0;
  d_spine[1][0] = 0.0; d_spine[1][1] = 0.0; d_spine[1][2] = 0.0;
  d_spine[2][0] = 0.0; d_spine[2][1] = 0.0; d_spine[2][2] = 0.0;

  d_head[0][0] = 0.0; d_head[0][1] = 0.0; d_head[0][2] = 0.0;

  d_shoulders[0][0] = 0.0; d_shoulders[0][1] = 0.0; d_shoulders[0][2] = 0.0;
  d_shoulders[1][0] = 0.0; d_shoulders[1][1] = 0.0; d_shoulders[1][2] = 0.0;
  d_shoulders[2][0] = 0.0; d_shoulders[2][1] = 0.0; d_shoulders[2][2] = 0.0;
  d_shoulders[3][0] = 0.0; d_shoulders[3][1] = 0.0; d_shoulders[3][2] = 0.0;
  d_shoulders[4][0] = 0.0; d_shoulders[4][1] = 0.0; d_shoulders[4][2] = 0.0;
  d_shoulders[5][0] = 0.0; d_shoulders[5][1] = 0.0; d_shoulders[5][2] = 0.0;

  d_elbows[0][0] = 0.0; d_elbows[0][1] = 0.0; d_elbows[0][2] = 0.0;
  d_elbows[1][0] = 0.0; d_elbows[1][1] = 0.0; d_elbows[1][2] = 0.0;
  d_elbows[2][0] = 0.0; d_elbows[2][1] = 0.0; d_elbows[2][2] = 0.0;
  d_elbows[3][0] = 0.0; d_elbows[3][1] = 0.0; d_elbows[3][2] = 0.0;
  d_elbows[4][0] = 0.0; d_elbows[4][1] = 0.0; d_elbows[4][2] = 0.0;
  d_elbows[5][0] = 0.0; d_elbows[5][1] = 0.0; d_elbows[5][2] = 0.0;

  d_legEnds[0][0] = 0.0; d_legEnds[0][1] = 0.0; d_legEnds[0][2] = 0.0;
  d_legEnds[1][0] = 0.0; d_legEnds[1][1] = 0.0; d_legEnds[1][2] = 0.0;
  d_legEnds[2][0] = 0.0; d_legEnds[2][1] = 0.0; d_legEnds[2][2] = 0.0;
  d_legEnds[3][0] = 0.0; d_legEnds[3][1] = 0.0; d_legEnds[3][2] = 0.0;
  d_legEnds[4][0] = 0.0; d_legEnds[4][1] = 0.0; d_legEnds[4][2] = 0.0;
  d_legEnds[5][0] = 0.0; d_legEnds[5][1] = 0.0; d_legEnds[5][2] = 0.0;

  return;
}
