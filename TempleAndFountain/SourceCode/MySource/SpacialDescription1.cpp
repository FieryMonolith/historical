#include <cmath>

#include "spacial_description.h"

SpacialDescription::SpacialDescription(void) {
  d_data.Identity();

  return;
}

SpacialDescription::SpacialDescription(const Vector3d &position, 
				                               const Vector3d &right,
                        				       const Vector3d &up, 
				                               const Vector3d &forward) {
  Matrix4x4d p(1.0, 0.0, 0.0, position[0],
           0.0, 1.0, 0.0, position[1],
           0.0, 0.0, 1.0, position[2],
           0.0, 0.0, 0.0, 1.0);
  Matrix4x4d rotation(right[0],   right[1],   right[2],   0.0,
                      up[0],      up[1],      up[2],      0.0,
                      forward[0], forward[1], forward[2], 0.0,
                      0.0,        0.0,        0.0,        1.0);
  d_data = rotation * p;

  return;
}

SpacialDescription::SpacialDescription(const Vector3d &position) {
  d_data.Identity();
  d_data.At(0,3) = position[0];
  d_data.At(1,3) = position[1];
  d_data.At(2,3) = position[2];

  return;
}

SpacialDescription::SpacialDescription(const SpacialDescription &sd) {
  d_data = sd.d_data;

  return;
}

SpacialDescription::~SpacialDescription(void) {
  return;
}

SpacialDescription &SpacialDescription::operator=(const SpacialDescription &sd) {
  d_data = sd.d_data;

  return *this;
}

void SpacialDescription::PitchUp(double angle) {
  d_data = CalculateRotationMatrix(angle,0) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::PitchDown(double angle) {
  d_data = CalculateRotationMatrix(-angle,0) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::YawRight(double angle) {
  d_data = CalculateRotationMatrix(angle,1) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::YawLeft(double angle) {
  d_data = CalculateRotationMatrix(-angle,1) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::RollLeft(double angle) {
  d_data = CalculateRotationMatrix(angle,2) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::RollRight(double angle) {
  d_data = CalculateRotationMatrix(-angle,2) * d_data;
  Orthonormalize();

  return;
}

void SpacialDescription::MoveRight(double unit) {
  d_data.At(0,3) += unit * d_data.At(0,0);
  d_data.At(1,3) += unit * d_data.At(0,1);
  d_data.At(2,3) += unit * d_data.At(0,2);

  return;
}

void SpacialDescription::MoveLeft(double unit) {
  d_data.At(0,3) += -unit * d_data.At(0,0);
  d_data.At(1,3) += -unit * d_data.At(0,1);
  d_data.At(2,3) += -unit * d_data.At(0,2);

  return;
}

void SpacialDescription::MoveUp(double unit) {
  d_data.At(0,3) += unit * d_data.At(1,0);
  d_data.At(1,3) += unit * d_data.At(1,1);
  d_data.At(2,3) += unit * d_data.At(1,2);

  return;
}

void SpacialDescription::MoveDown(double unit) {
  d_data.At(0,3) += -unit * d_data.At(1,0);
  d_data.At(1,3) += -unit * d_data.At(1,1);
  d_data.At(2,3) += -unit * d_data.At(1,2);

  return;
}

void SpacialDescription::MoveForward(double unit) {
  d_data.At(0,3) += -unit * d_data.At(2,0);
  d_data.At(1,3) += -unit * d_data.At(2,1);
  d_data.At(2,3) += -unit * d_data.At(2,2);

  return;
}

void SpacialDescription::MoveBackward(double unit) {
  d_data.At(0,3) += unit * d_data.At(2,0);
  d_data.At(1,3) += unit * d_data.At(2,1);
  d_data.At(2,3) += unit * d_data.At(2,2);

  return;
}

#if 0
void SpacialDescription::PitchUpAboutPoint(const Vector3d &point, 
					   double angle) {
  FacePoint(point);
  PitchDown(angle);

  double d = Distance(d_data, point);
  d_data[0] = (-1.0 * d_data.At(0,2) * d) + point[0];
  d_data[1] = (-1.0 * d_data.At(1,2) * d) + point[1];
  d_data[2] = (-1.0 * d_data.At(2,2) * d) + point[2];

  return;
}

void SpacialDescription::PitchDownAboutPoint(const Vector3d &point, 
					     double angle) {
  FacePoint(point);
  PitchUp(angle);

  double d = Distance(d_data, point);
  d_data[0] = (-1.0 * d_data.At(0,2) * d) + point[0];
  d_data[1] = (-1.0 * d_data.At(1,2) * d) + point[1];
  d_data[2] = (-1.0 * d_data.At(2,2) * d) + point[2];

  return;
}

void SpacialDescription::YawLeftAboutPoint(const Vector3d &point, 
					   double angle) {
  FacePoint(point);
  YawRight(angle);

  double d = Distance(d_data, point);
  d_data[0] = (-1.0 * d_data.At(0,2) * d) + point[0];
  d_data[1] = (-1.0 * d_data.At(1,2) * d) + point[1];
  d_data[2] = (-1.0 * d_data.At(2,2) * d) + point[2];

  return;
}

void SpacialDescription::YawRightAboutPoint(const Vector3d &point, 
					    double angle) {
  FacePoint(point);
  YawLeft(angle);

  double d = Distance(d_data, point);
  d_data[0] = (-1.0 * d_data.At(0,2) * d) + point[0];
  d_data[1] = (-1.0 * d_data.At(1,2) * d) + point[1];
  d_data[2] = (-1.0 * d_data.At(2,2) * d) + point[2];

  return;
}

void SpacialDescription::FacePoint(const Vector3d &point) {
  if(point == d_data) { return; }

  Vector3d newForward(point[0] - d_data[0],
            		      point[1] - d_data[1],
		                  point[2] - d_data[2]);
  Normalize(newForward);

  Vector3d delta(newForward[0] - d_data.At(0,2),
		 newForward[1] - d_data.At(1,2),
		 newForward[2] - d_data.At(2,2));

  d_data.At(0,0) +=     delta[0];
  d_data.At(1,0) += -1.0*delta[1];
  d_data.At(2,0) += -1.0*delta[2];

  d_data.At(0,1) += -1.0*delta[0];
  d_data.At(1,1) +=      delta[1];
  d_data.At(2,1) += -1.0*delta[2];

  d_data.At(0,2) = newForward[0];
  d_data.At(1,2) = newForward[1];
  d_data.At(2,2) = newForward[2];

  d_data.Orthonormalize();

  return;
}
#endif

void SpacialDescription::Rotate(const Matrix4x4d &m) {
  d_data = m * d_data;

  return;
}

void SpacialDescription::Translate(const Matrix4x4d &delta) {
  d_data = d_data * delta;
  return;
}

int operator==(const SpacialDescription &sd1, const SpacialDescription &sd2) {
  if(sd1.d_data == sd2.d_data) {
    return 1;
  }

  return 0;
}

int operator!=(const SpacialDescription &sd1, const SpacialDescription &sd2) {
  if(sd1.d_data != sd2.d_data) {
    return 1;
  }

  return 0;
}

void SpacialDescription::Orthonormalize(void) {
  Vector3d forward(d_data.At(0,0), d_data.At(0,1), d_data.At(0,2));
  Vector3d up(d_data.At(1,0), d_data.At(1,1), d_data.At(1,2));
  Vector3d out(d_data.At(2,0), d_data.At(2,1), d_data.At(2,2));

  Normalize(forward);
  Normalize(up);
  Normalize(out);

  d_data.At(0,0) = forward[0];
  d_data.At(0,1) = forward[1];
  d_data.At(0,2) = forward[2];

  d_data.At(1,0) = up[0];
  d_data.At(1,1) = up[1];
  d_data.At(1,2) = up[2];

  d_data.At(2,0) = out[0];
  d_data.At(2,1) = out[1];
  d_data.At(2,2) = out[2];

  return;
}

Matrix4x4d SpacialDescription::CalculateRotationMatrix(double angle, unsigned int axis) {
  double s = sin(angle);
  double c = cos(angle);

  switch(axis) {
  case 0:
    // Pitch
    return Matrix4x4d(1.0, 0.0, 0.0, 0.0,
                      0.0,   c,   s, 0.0,
                      0.0,  -s,   c, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  case 1:
    // Yaw
    return Matrix4x4d(  c, 0.0,   s, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                       -s, 0.0,   c, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  case 2:
    // Roll
    return Matrix4x4d(  c,   s, 0.0, 0.0,
                       -s,   c, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  }

  return Matrix4x4d();
}