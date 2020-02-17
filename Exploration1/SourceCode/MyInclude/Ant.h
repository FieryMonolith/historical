#ifndef __ANT_H__
#define __ANT_H__

class Vector3d;

class Ant {
public:
  Ant(void);
  ~Ant(void);

  void Draw(void) const;
  void Pose(const Vector3d &frontRight,  const Vector3d &frontLeft, 
            const Vector3d &middleRight, const Vector3d &middleLeft, 
            const Vector3d &backRight,   const Vector3d &backLeft);

private:
  double d_pointSize;
  double d_lineWidth;
  unsigned int d_numberOfLegs;

  double d_neckLength, d_tailLength;
  double d_lowerArmLength;
  double d_upperArmLength;
  double d_shoulderWidth, d_halfShoulderWidth;
  double d_shoulderSpacingLength;

  Vector3d *d_legEnds;
  Vector3d *d_elbows;
  Vector3d *d_shoulders;
  Vector3d *d_spine;
  Vector3d *d_head;
  Vector3d *d_tail;

  bool d_valid;

  void Zero(void);
};

#endif
