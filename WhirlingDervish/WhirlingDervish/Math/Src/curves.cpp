#include "jvector.h"

#include "curves.h"

bool JCurves::CatmullRomPosition(Vector3d &out, double t, const Vector3d &p0, 
				 const Vector3d &p1, const Vector3d &p2, 
				 const Vector3d &p3) {
  Vector3d tangent0(p2 - p0);
  Normalize(tangent0);
  Vector3d tangent1(p3 - p1);
  Normalize(tangent1);

  return Ferguson(out, t, p1, p2, tangent0, tangent1);
}

bool JCurves::CatmullRomTangent(Vector3d &out, double t, const Vector3d &p0, 
				const Vector3d &p1, const Vector3d &p2, 
				const Vector3d &p3) {
  Vector3d tangent0(p2 - p0);
  Normalize(tangent0);
  Vector3d tangent1(p3 - p1);
  Normalize(tangent1);

  return FergusonTangent(out, t, p1, p2, tangent0, tangent1);
}

bool JCurves::FergusonPosition(Vector3d &out, double t, const Vector3d &p0, 
			       const Vector3d &p1, const Vector3d &tangent0, 
			       const Vector3d &tangent1) {
  if(t < 0) {
    out = p0;
    return false;
  } else if(t > 1) {
    out = p1;
    return false;
  }
  double t2 = t * t;
  double t3 = t2 * t;

  /*
    P(t)  = a0 + a1*t + a2*t^2 + a3*t3
    P(0)  = a0
    P'(0) = a1
    P(1)  = a0 + a1 + a2 + a3
    P'(1) = a1 + 2*a2 + 3*a3

    P(U) = [1 t t^2 t^3][ 1  0  0  0 ][ P(0)  ]
                          0  0  1  0    P(1)
                         -3  3 -2 -1    P'(0)
                          2 -2  1  1    P'(1)

    out is everything multiplied together

    a0 = P(0)
    a1 = P'(0)
    a2 = 3*P(1) - 3*P(0) - 2*P'(0) - P'(1)
    a3 = 2*P(0) - 2*P(1) + P'(0) + P'(1)
  */

  double i0 = 1.0 +     (-3.0 * t2) + (2.0 * t3);
  double i1 =           ( 3.0 * t2) + (-2.0 * t3);
  double i2 =       t + (-2.0 * t2) + t3;
  double i3 =           -t2         + t3;

  out = (i0 * p0) + (i1 * p1) + (i2 * tangent0) + (i3 * tangent1);

  return true;
}

bool JCurves::FergusonTangent(Vector3d &out, double t, const Vector3d &p0, 
			      const Vector3d &p1, const Vector3d &tangent0, 
			      const Vector3d &tangent1) {
  if(t < 0) {
    out = p0;
    return false;
  } else if(t > 1) {
    out = p1;
    return false;
  }

  /*
    P(t)  = a0 + a1*t + a2*t^2 + a3*t3
    P(0)  = a0
    P'(0) = a1
    P(1)  = a0 + a1 + a2 + a3
    P'(1) = a1 + 2*a2 + 3*a3

    P(U) = [1 t t^2 t^3][ 1  0  0  0 ][ P(0)  ]
                          0  0  1  0    P(1)
                         -3  3 -2 -1    P'(0)
                          2 -2  1  1    P'(1)

    out = P'(t) = a1 + 2*a2*t + 3*a3*t^2

    a0 = P(0)
    a1 = P'(0)
    a2 = 3*P(1) - 3*P(0) - 2*P'(0) - P'(1)
    a3 = 2*P(0) - 2*P(1) + P'(0) + P'(1)
  */

  out = tangent0 +
        (2.0 * ((3.0 * p1) - (3.0 * p0) - (2.0 * tangent0) - tangent1) * t) +
        (3.0 * ((2.0 * p0) - (2.0 * p1) + tangent0 + tangent1) * t * t);

  return true;
}
