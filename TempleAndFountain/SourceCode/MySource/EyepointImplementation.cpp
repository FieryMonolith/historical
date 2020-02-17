/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * EyepointImplementation - These are the implementation details of the
 *   eyepoint.  A spacial description is used to remember the current 
 *   position and orientatin of the eyepoint.  This class was extracted
 *   from one of my other projects, and only FLY mode is available.

 */

#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "EyepointImplementation.h"

EyepointImplementation::EyepointImplementation(void) 
  : d_mode(FLY), d_forwardSpeed(32.0), d_rotationalSpeed(3.14/180.0) {
  SetFixPoint(Vector3d(0.0, 0.0, 0.0));
  Vector3d &p = d_sd.Position();
  p[2] = -1100;

  return;
}

EyepointImplementation::EyepointImplementation(double x, double y, double z,
                                               double outx, double outy, double outz,
                                               double upx, double upy, double upz)
: d_mode(FLY), d_forwardSpeed(8.0), d_rotationalSpeed(M_PI/180.0)
{
  SetFixPoint(Vector3d(0.0, 0.0, 0.0));
  Matrix4x4d &p = d_sd.PositionMatrix();
  p.At(0,3) = x;
  p.At(1,3) = y;
  p.At(2,3) = z;

  Vector3d up(upx, upy, upz);
  Normalize(up);
  Vector3d out(outx, outy, outz);
  Normalize(out);

  Vector3d right = Cross(out, up);
  Normalize(right);

  Matrix4x4d &m = d_sd.RotationMatrix();
  m.At(0,0) = right[0]; m.At(0,1) = right[1]; m.At(0,2) = right[2];
  m.At(1,0) = up[0]; m.At(1,1) = up[1]; m.At(1,2) = up[2];
  m.At(2,0) = out[0]; m.At(2,1) = out[1]; m.At(2,2) = out[2];

  return;
}

EyepointImplementation::~EyepointImplementation(void) {
  return;
}

// Prepares the GL_MODELVIEW for opengl
void EyepointImplementation::Draw(void) const {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  Matrix4x4d temp = d_sd.RTrMatrix();
  double m[16];
  m[0] = temp.At(0,0); m[4] = temp.At(0,1); m[8]  = temp.At(0,2); m[12] = temp.At(0,3);
  m[1] = temp.At(1,0); m[5] = temp.At(1,1); m[9]  = temp.At(1,2); m[13] = temp.At(1,3);
  m[2] = temp.At(2,0); m[6] = temp.At(2,1); m[10] = temp.At(2,2); m[14] = temp.At(2,3);
  m[3] = temp.At(3,0); m[7] = temp.At(3,1); m[11] = temp.At(3,2); m[15] = temp.At(3,3);
  glLoadMatrixd(m);

  return;
}

Vector3d EyepointImplementation::RightVector(void) const {
  return d_sd.Right();
}

Vector3d EyepointImplementation::UpVector(void) const {
  return d_sd.Up();
}

Vector3d EyepointImplementation::OutVector(void) const {
  return d_sd.Forward();
}

Vector3d EyepointImplementation::Location(void) const {
  return d_sd.Position();
}

void EyepointImplementation::IncreaseRotationSpeed(void) {
  d_rotationalSpeed += M_PI / 180.0;
  if(d_rotationalSpeed > 360.0) {
    d_rotationalSpeed = 360.0;
  }

  return;
}

void EyepointImplementation::DecreaseRotationSpeed(void) {
  d_rotationalSpeed -= M_PI / 180.0;
  if(d_rotationalSpeed < M_PI / 180.0) {
    d_rotationalSpeed = M_PI / 180.0;
  }

  return;
}

void EyepointImplementation::IncreaseForwardSpeed(void) {
  d_forwardSpeed *= 2.0;
  if(d_forwardSpeed > 512) {
    d_forwardSpeed = 512.0;
  }

  return;
}

void EyepointImplementation::DecreaseForwardSpeed(void) {
  d_forwardSpeed /= 2.0;
  if(d_forwardSpeed < 0.03125) {
    d_forwardSpeed = 0.03125;
  }

  return;
}

void EyepointImplementation::MoveForward(void) {
  EyepointForward();

  return;
}

void EyepointImplementation::MoveBackward(void) {
  EyepointBack();

  return;
}

void EyepointImplementation::MoveLeft(void) {
  EyepointLeft();

  return;
}

void EyepointImplementation::MoveRight(void) {
  EyepointRight();

  return;
}

void EyepointImplementation::MoveUp(void) {
  EyepointUp();

  return;
}

void EyepointImplementation::MoveDown(void) {
  EyepointDown();

  return;
}

void EyepointImplementation::YawLeft(void) {
  if(d_mode == FLY) {
    EyepointYawLeft();
  } else {
    EyefixYawLeft();
  }

  return;
}

void EyepointImplementation::YawRight(void) {
  if(d_mode == FLY) {
    EyepointYawRight();
  } else {
    EyefixYawRight();
  }

  return;
}

void EyepointImplementation::PitchUp(void) {
  if(d_mode == FLY) {
    EyepointPitchUp();
  } else {
    EyefixPitchUp();
  }

  return;
}

void EyepointImplementation::PitchDown(void) {
  if(d_mode == FLY) {
    EyepointPitchDown();
  } else {
    EyefixPitchDown();
  }

  return;
}

void EyepointImplementation::RollLeft(void) {
  EyepointRollLeft();

  return;
}

void EyepointImplementation::RollRight(void) {
  EyepointRollRight();

  return;
}

void EyepointImplementation::SetFixPoint(const Vector3d &fix) {
  d_fix_pt = fix;

  return;
}

void EyepointImplementation::Fly(void) {
  d_mode = FLY;

  return;
}

void EyepointImplementation::RotateCenter(void) {
  d_mode = CENTER;
  return;
}

void EyepointImplementation::Follow(Vector3d *v) {
  d_followPoint = 0;
  if(v != 0) {
    d_followPoint = v;
    d_mode = FOLLOW;
  }

  return;
}

void EyepointImplementation::EyepointLeft(void) {
  d_sd.MoveLeft(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyepointRight(void) {
  d_sd.MoveRight(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyepointUp(void) {
  d_sd.MoveUp(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyepointDown(void) {
  d_sd.MoveDown(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyepointForward(void) {
  d_sd.MoveForward(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyepointBack(void) {
  d_sd.MoveBackward(d_forwardSpeed);

  return;
}

void EyepointImplementation::EyefixYawLeft(void) {
  d_sd.YawLeftAboutPoint(d_fix_pt, d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyefixYawRight(void) {
  d_sd.YawRightAboutPoint(d_fix_pt, d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyefixPitchUp(void) {
  d_sd.PitchUpAboutPoint(d_fix_pt, d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyefixPitchDown(void) {
  d_sd.PitchDownAboutPoint(d_fix_pt, d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointYawLeft(void) {
  d_sd.YawLeft(d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointYawRight(void) {
  d_sd.YawRight(d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointPitchUp(void) {
  d_sd.PitchUp(d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointPitchDown(void) {
  d_sd.PitchDown(d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointRollLeft(void) {
  d_sd.RollLeft(d_rotationalSpeed);

  return;
}

void EyepointImplementation::EyepointRollRight(void) {
  d_sd.RollRight(d_rotationalSpeed);

  return;
}

// Changes the mode between FLY and CENTER, where CENTER is supposed to
// focus the eyepoint on a point and rotate about it.
void EyepointImplementation::ToggleView(void) {
  if(d_mode == FLY) {
    d_mode = CENTER;
  } else if(d_mode == CENTER) {
    d_mode = FLY;
  }

  return;
}
