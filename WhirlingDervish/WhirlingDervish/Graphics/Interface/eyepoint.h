#ifndef __EYEPOINT_H__
#define __EYEPOINT_H__

class Vector3d;

class Eyepoint {
public:
  Eyepoint(void) { return; }
  virtual ~Eyepoint(void) { return; }

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
  virtual void Fly(void)=0;
  virtual void RotateCenter(void)=0;
  virtual void Follow(Vector3d *)=0;
};

#endif
