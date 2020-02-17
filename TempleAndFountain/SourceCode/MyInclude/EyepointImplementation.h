/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __EYEPOINT_IMPLEMENTATION_H__
#define __EYEPOINT_IMPLEMENTATION_H__

#include "Eyepoint.h"

#include "SpacialDescription.h"

/*

 * EyepointImplementation - These are the implementation details of the
 *   eyepoint.  A spacial description is used to remember the current 
 *   position and orientatin of the eyepoint.  This class was extracted
 *   from one of my other projects, and only FLY mode is available.

 */
class EyepointImplementation : public Eyepoint {
public:
  EyepointImplementation();
  EyepointImplementation(double x, double y, double z,
                         double outx, double outy, double outz,
                         double upx, double upy, double upz);
  ~EyepointImplementation(void);

  // Prepares the GL_MODELVIEW for opengl
  void Draw(void) const;

  // Accessor functions that describe the position and orientation of the
  //   eyepoint.
  Vector3d RightVector(void) const;
  Vector3d UpVector(void) const;
  Vector3d OutVector(void) const;
  Vector3d Location(void) const;

  // Interface functions that override the virtual inherited Eyepoint versions
  void IncreaseRotationSpeed(void);
  void DecreaseRotationSpeed(void);
  void IncreaseForwardSpeed(void);
  void DecreaseForwardSpeed(void);

  // Interface functions that override the virtual inherited Eyepoint versions
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
  void RollLeft(void);
  void RollRight(void);

  // Changes the mode between FLY and CENTER, where CENTER is supposed to
  // focus the eyepoint on a point and rotate about it.
  void ToggleView(void);

  // Sets the point about which CENTER will rotate
  void SetFixPoint(const Vector3d &);

  void Fly(void);          // Set mode to FLY
  void RotateCenter(void); // Set mode to CENTER
  void Follow(Vector3d *); // Set mode to FOLLOW

private:
  enum { FLY, CENTER, FOLLOW }; // The different eyepoint movement modes

  unsigned int d_mode; // current mode
  double d_forwardSpeed, d_rotationalSpeed;

  Vector3d *d_followPoint;
  Vector3d d_fix_pt;

  SpacialDescription d_sd;

  // The functions that do the work
  void EyepointLeft(void);
  void EyepointRight(void);
  void EyepointUp(void);
  void EyepointDown(void);
  void EyepointForward(void);
  void EyepointBack(void);

  void EyefixYawLeft(void);
  void EyefixYawRight(void);
  void EyefixPitchUp(void);
  void EyefixPitchDown(void);

  void EyepointYawLeft(void);
  void EyepointYawRight(void);
  void EyepointPitchUp(void);
  void EyepointPitchDown(void);
  void EyepointRollLeft(void);
  void EyepointRollRight(void);
};

#endif
