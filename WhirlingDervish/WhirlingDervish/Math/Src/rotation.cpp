#include <cmath>

#include "matrix.h"
#include "jvector.h"

#include "rotation.h"

Matrix4x4d RotationFromZ(const Vector3d &to) {
  double theta = 0.0;
  double phi = 0.0;
  CalculateRotationAnglesToZ(theta, phi, to);

  theta = (2.0 * M_PI) - theta;
  phi = (2.0 * M_PI) - phi;

  double ct = cos(theta);
  double cp = cos(phi);
  double st = sin(theta);
  double sp = sin(phi);
  return Matrix4x4d( ct,  sp*st,  st*cp, 0.0,
                    0.0,     cp,    -sp, 0.0,
                    -st,   sp*ct,  cp*ct, 0.0,
                    0.0,    0.0,    0.0, 1.0);
}

Matrix4x4d RotationToZ(const Vector3d &from) {
  double theta = 0.0;
  double phi = 0.0;
  CalculateRotationAnglesToZ(theta, phi, from);

  double ct = cos(theta);
  double cp = cos(phi);
  double st = sin(theta);
  double sp = sin(phi);
  return Matrix4x4d(    ct, 0.0,     st, 0.0,
                     sp*st,  cp, -sp*ct, 0.0,
                    -st*cp,  sp,  cp*ct, 0.0,
                       0.0, 0.0,    0.0, 1.0);
}

void CalculateRotationAnglesToZ(double &theta, double &phi, 
				const Vector3d &direction) {
  if(direction[0] == 0 && direction[1] == 0 && direction[2] == 0) {
    theta = 0.0;
    phi = 0.0;
    return;
  }
  double xc = direction[0];
  if(xc < 0) { xc *= -1.0; }
  double yc = direction[1];
  if(yc < 0) { yc *= -1.0; }
  double zc = direction[2];
  if(zc < 0) { zc *= -1.0; }
  if(xc < 0.00001 && direction[1] > 0.99999 && zc < 0.00001) {
    phi = M_PI / 2.0;
    theta = 0.0;
    return;
  } else if(xc < 0.00001 && direction[1] < -0.99999 && zc < 0.00001) {
    phi = 3.0 * M_PI / 2.0;
    theta = 0.0;
    return;
  }

  Vector3d z(0.0, 0.0, 1.0);
  Vector3d v(direction);
  Normalize(v);

  Vector3d thetaV(v[0], 0.0, v[2]);
  Normalize(thetaV);

  Vector3d phiV(0.0, v[1], v[2]);
  Normalize(phiV);

  theta = acos(Inner(z, thetaV));
  if(thetaV[0] > 0) {
    theta = (2.0 * M_PI) - theta;
  }

  if(phiV[2] < 0) {
    phiV[2] *= -1.0;
  }

  if(xc > 0.00001 && yc > 0.00001 && zc < 0.00001) {
    phiV[0] = xc;
    phiV[2] = 0.0;
    z = Vector3d(1.0, 0.0, 0.0);
  }

  phi = acos(Inner(z, phiV));
  if(phiV[1] == 0) {
    phi = 0.0;
  } else if(phiV[1] < 0) {
    phi = (2.0 * M_PI) - phi;
  }

  return;
}

void Orthonormalize(Matrix4x4d &m) {
  Vector3d left(m.At(0,0), m.At(0,1), m.At(0,2));
  Vector3d up(m.At(1,0), m.At(1,1), m.At(1,2));

  Normalize(left);
  Normalize(up);

  Vector3d out = Cross(left, up);
  Normalize(out);

  left = Cross(up, out);
  Normalize(left);

  m.At(0,0) = left[0];
  m.At(0,1) = left[1];
  m.At(0,2) = left[2];

  m.At(1,0) = up[0];
  m.At(1,1) = up[1];
  m.At(1,2) = up[2];

  m.At(2,0) = out[0];
  m.At(2,1) = out[1];
  m.At(2,2) = out[2];

  return;
}
