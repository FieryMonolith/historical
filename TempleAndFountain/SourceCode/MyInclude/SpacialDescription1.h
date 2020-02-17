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

  Vector3d Position(void) const { return Vector3d(d_data.At(0,3),
                                                  d_data.At(1,3),
                                                  d_data.At(2,3)); }
  Vector3d Right(void) const   { return Vector3d(d_data.At(0,0),
                                    						 d_data.At(0,1),
						                                     d_data.At(0,2)); }
  Vector3d Up(void) const      { return Vector3d(d_data.At(1,0),
                                    						 d_data.At(1,1),
                                    						 d_data.At(1,2)); }
  Vector3d Forward(void) const { return Vector3d(d_data.At(2,0),
                                    						 d_data.At(2,1),
                                    						 d_data.At(2,2)); }

  Matrix4x4d RotationMatrix(void) const { return Matrix4x4d(d_data.At(0,0), d_data.At(0,1), d_data.At(0,2), 0.0,
                                                            d_data.At(1,0), d_data.At(1,1), d_data.At(1,2), 0.0,
                                                            d_data.At(2,0), d_data.At(2,1), d_data.At(2,2), 0.0,
                                                            0.0,            0.0,            0.0,            1.0); }
  Matrix4x4d InverseRotationMatrix(void) const { return Transpose(RotationMatrix()); }
  Matrix4x4d PositionMatrix(void) const { return Matrix4x4d(1.0, 0.0, 0.0, d_data.At(0,3),
                                                            0.0, 1.0, 0.0, d_data.At(1,3),
                                                            0.0, 0.0, 1.0, d_data.At(2,3),
                                                            0.0, 0.0, 0.0, 1.0); }
  Matrix4x4d InversePositionMatrix(void) const{ return Matrix4x4d(1.0, 0.0, 0.0, -1.0*d_data.At(0,3),
                                                                  0.0, 1.0, 0.0, -1.0*d_data.At(1,3),
                                                                  0.0, 0.0, 1.0, -1.0*d_data.At(2,3),
                                                                  0.0, 0.0, 0.0, 1.0); }

  const Matrix4x4d &Matrix(void) const { return d_data; }
  Matrix4x4d &Matrix(void) { return d_data; }

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

#if 0
  void PitchUpAboutPoint(const Vector3d &point, double angle);
  void PitchDownAboutPoint(const Vector3d &point, double angle);
  void YawLeftAboutPoint(const Vector3d &point, double angle);
  void YawRightAboutPoint(const Vector3d &point, double angle);
  void RollLeftAboutPoint(const Vector3d &point, double angle);
  void RollRightAboutPoint(const Vector3d &point, double angle);
  void FacePoint(const Vector3d &point);
#endif

  void Rotate(const Matrix4x4d &m);
  void Translate(const Matrix4x4d &delta);

private:
  Matrix4x4d d_data;

  friend int operator==(const SpacialDescription &,const SpacialDescription &);
  friend int operator!=(const SpacialDescription &,const SpacialDescription &);
  void Orthonormalize(void);
  Matrix4x4d CalculateRotationMatrix(double angle, unsigned int axis);
};

int operator==(const SpacialDescription &, const SpacialDescription &);
int operator!=(const SpacialDescription &, const SpacialDescription &);

#endif
