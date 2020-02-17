/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __EYEPOINT_H__
#define __EYEPOINT_H__

class Vector3d;

/*

 * Eyepoint - This class is a pure interface class that is used to hide the
 *   implementation details of an eyepoint.

 */

class Eyepoint {
public:
  Eyepoint(void) { return; }
  ~Eyepoint(void) { return; }

  virtual void Draw(void) const=0;
  virtual void IncreaseRotationSpeed(void)=0;
  virtual void DecreaseRotationSpeed(void)=0;
  virtual void IncreaseForwardSpeed(void)=0;
  virtual void DecreaseForwardSpeed(void)=0;

  virtual void MoveForward(void)=0;
  virtual void MoveBackward(void)=0;
  virtual void MoveLeft(void)=0;
  virtual void MoveRight(void)=0;
  virtual void MoveUp(void)=0;
  virtual void MoveDown(void)=0;
  virtual void YawLeft(void)=0;
  virtual void YawRight(void)=0;
  virtual void PitchUp(void)=0;
  virtual void PitchDown(void)=0;
  virtual void RollLeft(void)=0;
  virtual void RollRight(void)=0;

  virtual void ToggleView(void)=0;
  virtual void SetFixPoint(const Vector3d &)=0;
};

#endif
