#include <cmath>

#include "spacial_description.h"

SpacialDescription::SpacialDescription(void) {
  d_orientation.Identity();
  d_position.Identity();

  return;
}

SpacialDescription::SpacialDescription(const Vector3d &position,
				       const Vector3d &right,
				       const Vector3d &up,
				       const Vector3d &forward) {
  d_position.Identity();
  d_position.At(0,3) = position[0];
  d_position.At(1,3) = position[1];
  d_position.At(2,3) = position[2];
  d_orientation = Matrix4x4d(right[0],   right[1],   right[2],   0.0,
                             up[0],      up[1],      up[2],      0.0,
                             forward[0], forward[1], forward[2], 0.0,
                             0.0,        0.0,        0.0,        1.0);

  return;
}

SpacialDescription::SpacialDescription(const Vector3d &position) {
  d_orientation.Identity();
  d_position.Identity();
  d_position.At(0,3) = position[0];
  d_position.At(1,3) = position[1];
  d_position.At(2,3) = position[2];

  return;
}

SpacialDescription::SpacialDescription(const SpacialDescription &sd) {
  d_orientation = sd.d_orientation;
  d_position = sd.d_position;

  return;
}

SpacialDescription::~SpacialDescription(void) {
  return;
}

SpacialDescription &SpacialDescription::operator=(const SpacialDescription &sd)
{
  d_orientation = sd.d_orientation;
  d_position = sd.d_position;

  return *this;
}

void SpacialDescription::SetPosition(const Vector3d &p) {
  d_position.At(0,3) = p[0];
  d_position.At(1,3) = p[1];
  d_position.At(2,3) = p[2];

  return;
}

void SpacialDescription::SetOrientation(const Vector3d &o, const Vector3d &u) {
  Vector3d right = Cross(o, u);

  d_orientation.At(0,0) = right[0];
  d_orientation.At(0,1) = right[1];
  d_orientation.At(0,2) = right[2];
  d_orientation.At(1,0) = u[0];
  d_orientation.At(1,1) = u[1];
  d_orientation.At(1,2) = u[2];
  d_orientation.At(2,0) = o[0];
  d_orientation.At(2,1) = o[1];
  d_orientation.At(2,2) = o[2];

  Orthonormalize();

  return;
}

void SpacialDescription::PitchUp(double angle) {
  d_orientation = CalculateRotationMatrix(angle,PITCH) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::PitchDown(double angle) {
  d_orientation = CalculateRotationMatrix(-angle,PITCH) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::YawRight(double angle) {
  d_orientation = CalculateRotationMatrix(angle,YAW) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::YawLeft(double angle) {
  d_orientation = CalculateRotationMatrix(-angle,YAW) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::RollLeft(double angle) {
  d_orientation = CalculateRotationMatrix(angle,ROLL) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::RollRight(double angle) {
  d_orientation = CalculateRotationMatrix(-angle,ROLL) * d_orientation;
  Orthonormalize();

  return;
}

void SpacialDescription::MoveRight(double unit) {
  d_position.At(0,3) += unit * d_orientation.At(0,0);
  d_position.At(1,3) += unit * d_orientation.At(0,1);
  d_position.At(2,3) += unit * d_orientation.At(0,2);

  return;
}

void SpacialDescription::MoveLeft(double unit) {
  d_position.At(0,3) += -unit * d_orientation.At(0,0);
  d_position.At(1,3) += -unit * d_orientation.At(0,1);
  d_position.At(2,3) += -unit * d_orientation.At(0,2);

  return;
}

void SpacialDescription::MoveUp(double unit) {
  d_position.At(0,3) += unit * d_orientation.At(1,0);
  d_position.At(1,3) += unit * d_orientation.At(1,1);
  d_position.At(2,3) += unit * d_orientation.At(1,2);

  return;
}

void SpacialDescription::MoveDown(double unit) {
  d_position.At(0,3) += -unit * d_orientation.At(1,0);
  d_position.At(1,3) += -unit * d_orientation.At(1,1);
  d_position.At(2,3) += -unit * d_orientation.At(1,2);

  return;
}

void SpacialDescription::MoveForward(double unit) {
  d_position.At(0,3) += unit * d_orientation.At(2,0);
  d_position.At(1,3) += unit * d_orientation.At(2,1);
  d_position.At(2,3) += unit * d_orientation.At(2,2);

  return;
}

void SpacialDescription::MoveBackward(double unit) {
  d_position.At(0,3) += -unit * d_orientation.At(2,0);
  d_position.At(1,3) += -unit * d_orientation.At(2,1);
  d_position.At(2,3) += -unit * d_orientation.At(2,2);

  return;
}

void SpacialDescription::PitchUpAboutPoint(const Vector3d &point, 
					   double angle) {
  FacePoint(point);
  PitchUp(angle);

  Vector3d origin(0.0, 0.0, 0.0);
  Vector3d location(d_position.At(0,3), d_position.At(1,3), 
		    d_position.At(2,3));
  double mag = Distance(origin, location);

  d_position.At(0,3) = -mag * d_orientation.At(2,0);
  d_position.At(1,3) = -mag * d_orientation.At(2,1);
  d_position.At(2,3) = -mag * d_orientation.At(2,2);

  return;
}

void SpacialDescription::PitchDownAboutPoint(const Vector3d &point, 
					     double angle) {
  FacePoint(point);
  PitchUp(-angle);

  Vector3d origin(0.0, 0.0, 0.0);
  Vector3d location(d_position.At(0,3), d_position.At(1,3), 
		    d_position.At(2,3));
  double mag = Distance(origin, location);

  d_position.At(0,3) = -mag * d_orientation.At(2,0);
  d_position.At(1,3) = -mag * d_orientation.At(2,1);
  d_position.At(2,3) = -mag * d_orientation.At(2,2);

  return;
}

void SpacialDescription::YawLeftAboutPoint(const Vector3d &point, 
					   double angle) {
  FacePoint(point);
  YawLeft(-angle);

  Vector3d origin(0.0, 0.0, 0.0);
  Vector3d location(d_position.At(0,3), d_position.At(1,3), 
		    d_position.At(2,3));
  double mag = Distance(origin, location);

  d_position.At(0,3) = -mag * d_orientation.At(2,0);
  d_position.At(1,3) = -mag * d_orientation.At(2,1);
  d_position.At(2,3) = -mag * d_orientation.At(2,2);

  return;
}

void SpacialDescription::YawRightAboutPoint(const Vector3d &point, 
					    double angle) {
  FacePoint(point);
  YawRight(-angle);

  Vector3d origin(0.0, 0.0, 0.0);
  Vector3d location(d_position.At(0,3), d_position.At(1,3), 
		    d_position.At(2,3));
  double mag = Distance(origin, location);

  d_position.At(0,3) = -mag * d_orientation.At(2,0);
  d_position.At(1,3) = -mag * d_orientation.At(2,1);
  d_position.At(2,3) = -mag * d_orientation.At(2,2);

  return;
}

void SpacialDescription::FacePoint(const Vector3d &point) {
  Vector3d location(d_position.At(0,3), d_position.At(1,3), 
		    d_position.At(2,3));
  if(location == point) { return; }

#if 0
  Vector3d newForward(point[0] - location[0],
            		      point[1] - location[1],
		                  point[2] - location[2]);
  Normalize(newForward);

  Vector3d delta(newForward[0] - d_orientation.At(2,0),
		             newForward[1] - d_orientation.At(2,1),
		             newForward[2] - d_orientation.At(2,2));

  d_orientation.At(0,0) +=     delta[0];
  d_orientation.At(0,1) += -1.0*delta[1];
  d_orientation.At(0,2) += -1.0*delta[2];

  d_orientation.At(1,0) += -1.0*delta[0];
  d_orientation.At(1,1) +=      delta[1];
  d_orientation.At(1,2) += -1.0*delta[2];

  d_orientation.At(2,0) = newForward[0];
  d_orientation.At(2,1) = newForward[1];
  d_orientation.At(2,2) = newForward[2];

  Orthonormalize();
#endif

  return;
}

void SpacialDescription::Rotate(const Matrix4x4d &m) {
  d_orientation = m * d_orientation;

  return;
}

void SpacialDescription::Translate(const Vector3d &delta) {
  d_position.At(0,3) += delta[0];
  d_position.At(1,3) += delta[1];
  d_position.At(2,3) += delta[2];

  return;
}

int operator==(const SpacialDescription &sd1, const SpacialDescription &sd2) {
  if(sd1.d_position == sd2.d_position &&
     sd1.d_orientation == sd2.d_orientation) {
    return 1;
  }

  return 0;
}

int operator!=(const SpacialDescription &sd1, const SpacialDescription &sd2) {
  if(sd1.d_position != sd2.d_position ||
     sd1.d_orientation != sd2.d_orientation) {
    return 1;
  }

  return 0;
}

void SpacialDescription::Orthonormalize(void) {
  Vector3d right(d_orientation.At(0,0), d_orientation.At(0,1), 
		   d_orientation.At(0,2));
  Vector3d up(d_orientation.At(1,0), d_orientation.At(1,1), 
	      d_orientation.At(1,2));

  Normalize(right);
  Normalize(up);

#if 1
  // NEED to verify that the vectors are still perpendicular to each other

  // Can get funny stuff with opengl, because z is into the screen instead
  // of out of the screen

  Vector3d out = Cross(up, right);
  Normalize(out);

  right = Cross(out, up);
  Normalize(right);
#else
  Vector3d out(d_orientation.At(2,0), d_orientation.At(2,1), 
	       d_orientation.At(2,2));
  Normalize(out);
#endif

  d_orientation.At(0,0) = right[0];
  d_orientation.At(0,1) = right[1];
  d_orientation.At(0,2) = right[2];

  d_orientation.At(1,0) = up[0];
  d_orientation.At(1,1) = up[1];
  d_orientation.At(1,2) = up[2];

  d_orientation.At(2,0) = out[0];
  d_orientation.At(2,1) = out[1];
  d_orientation.At(2,2) = out[2];

  return;
}

Matrix4x4d SpacialDescription::CalculateRotationMatrix(double angle, 
						       unsigned int axis) {
  double s = sin(angle);
  double c = cos(angle);

  switch(axis) {
  case PITCH:
    return Matrix4x4d(1.0, 0.0, 0.0, 0.0,
                      0.0,   c,   s, 0.0,
                      0.0,  -s,   c, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  case YAW:
    return Matrix4x4d(  c, 0.0,   s, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                       -s, 0.0,   c, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  case ROLL:
    return Matrix4x4d(  c,   s, 0.0, 0.0,
                       -s,   c, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);
  }

  return Matrix4x4d();
}
