/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __SPACIAL_DESCRIPTION_H__
#define __SPACIAL_DESCRIPTION_H__

#include "Vector3d.h"
#include "Matrix.h"

class SpacialDescription {
public:
  SpacialDescription(void);
  SpacialDescription(const Vector3d &position, const Vector3d &right,
		     const Vector3d &up, const Vector3d &forward);
  SpacialDescription(const Vector3d &position);
  SpacialDescription(const SpacialDescription &);
  virtual ~SpacialDescription(void);

  SpacialDescription &operator=(const SpacialDescription &);

  Vector3d Position(void) const { return Vector3d(d_position.At(0,3),
                                                  d_position.At(1,3),
                                                  d_position.At(2,3)); }
  Vector3d Right(void) const   { return Vector3d(d_orientation.At(0,0),
                                    						 d_orientation.At(0,1),
						                                     d_orientation.At(0,2)); }
  Vector3d Up(void) const      { return Vector3d(d_orientation.At(1,0),
                                    						 d_orientation.At(1,1),
                                    						 d_orientation.At(1,2)); }
  Vector3d Forward(void) const { return Vector3d(d_orientation.At(2,0),
                                    						 d_orientation.At(2,1),
                                    						 d_orientation.At(2,2)); }

  const Matrix4x4d &RotationMatrix(void) const { return d_orientation; }
  Matrix4x4d &RotationMatrix(void) { return d_orientation; }
  Matrix4x4d InverseRotationMatrix(void) const { return Transpose(d_orientation); }

  const Matrix4x4d &PositionMatrix(void) const { return d_position; }
  Matrix4x4d &PositionMatrix(void) { return d_position; }
  Matrix4x4d InversePositionMatrix(void) const{ return Matrix4x4d(1.0, 0.0, 0.0, -1.0*d_position.At(0,3),
                                                                  0.0, 1.0, 0.0, -1.0*d_position.At(1,3),
                                                                  0.0, 0.0, 1.0, -1.0*d_position.At(2,3),
                                                                  0.0, 0.0, 0.0, 1.0); }

  Matrix4x4d TrRMatrix(void) const { return d_position * d_orientation; }
  Matrix4x4d RTrMatrix(void) const { return d_orientation * d_position; }

  void PitchUp(double angle);
  void PitchDown(double angle);
  void YawLeft(double angle);
  void YawRight(double angle);
  void RollLeft(double angle);
  void RollRight(double angle);
  void MoveRight(double unit);
  void MoveLeft(double unit);
  void MoveUp(double unit);
  void MoveDown(double unit);
  void MoveForward(double unit);
  void MoveBackward(double unit);

  void PitchUpAboutPoint(const Vector3d &point, double angle);
  void PitchDownAboutPoint(const Vector3d &point, double angle);
  void YawLeftAboutPoint(const Vector3d &point, double angle);
  void YawRightAboutPoint(const Vector3d &point, double angle);
  void RollLeftAboutPoint(const Vector3d &point, double angle);
  void RollRightAboutPoint(const Vector3d &point, double angle);
  void FacePoint(const Vector3d &point);

  void Rotate(const Matrix4x4d &m);
  void Translate(const Vector3d &delta);

private:
  Matrix4x4d d_orientation;
  Matrix4x4d d_position;

  friend int operator==(const SpacialDescription &,const SpacialDescription &);
  friend int operator!=(const SpacialDescription &,const SpacialDescription &);
  void Orthonormalize(void);
  Matrix4x4d CalculateRotationMatrix(double angle, unsigned int axis);
};

int operator==(const SpacialDescription &, const SpacialDescription &);
int operator!=(const SpacialDescription &, const SpacialDescription &);

#endif
