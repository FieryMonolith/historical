#ifndef __EYEPOINT_IMPLEMENTATION_H__
#define __EYEPOINT_IMPLEMENTATION_H__

#include "eyepoint.h"

#include "spacial_description.h"

class EyepointImplementation : public Eyepoint {
public:
  EyepointImplementation(void);
  EyepointImplementation(double x, double y, double z,
                         double outx, double outy, double outz,
                         double upx, double upy, double upz);
  ~EyepointImplementation(void);

  Vector3d RightVector(void) const;
  Vector3d UpVector(void) const;
  Vector3d OutVector(void) const;
  Vector3d Location(void) const;

  void Draw(void) const;

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
  void RollLeft(void);
  void RollRight(void);

  void ToggleView(void);

  void SetFixPoint(const Vector3d &);

  void Fly(void);
  void RotateCenter(void);
  void Follow(Vector3d *);

private:
  enum { FLY, CENTER, FOLLOW };

  unsigned int d_mode;
  double d_forwardSpeed, d_rotationalSpeed;

  Vector3d *d_followPoint;
  Vector3d d_fix_pt;

  SpacialDescription d_sd;

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
