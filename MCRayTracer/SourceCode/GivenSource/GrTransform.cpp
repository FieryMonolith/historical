// GrTransform.cpp: implementation of the CGrTransform class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cmath>
#include "GrTransform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



void CGrTransform::Identity()
{
   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
}

void CGrTransform::Zero()
{
   m[0][0] = 0;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 0;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 0;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 0;
}

//
// Rotation matrices.  There are two versions of each
// of the standard matrices, a version that accepts
// an angle IN DEGREES and a version that accepts
// the cosine and sine of the rotation angle.
//

CGrTransform &CGrTransform::RotateX(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = cr;  m[1][2] = -sr;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = sr;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform &CGrTransform::RotateX(double cr, double sr)
{
   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = cr;  m[1][2] = -sr;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = sr;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform &CGrTransform::RotateY(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = cr;  m[0][1] = 0;  m[0][2] = sr;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = -sr;  m[2][1] = 0;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform & CGrTransform::RotateY(double cr, double sr)
{
   m[0][0] = cr;  m[0][1] = 0;  m[0][2] = sr;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = -sr;  m[2][1] = 0;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}


CGrTransform &CGrTransform::RotateZ(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = cr;  m[0][1] = -sr;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = sr;  m[1][1] = cr;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform & CGrTransform::RotateZ(double cr, double sr)
{
   m[0][0] = cr;  m[0][1] = -sr;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = sr;  m[1][1] = cr;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

//
// Name :         CGrTransform::Rotate()
// Description :  Create a rotation matrix that will rotate
//                by angle r around vector v.  Note that
//                r is in degrees.
//

CGrTransform &CGrTransform::Rotate(double r, const CGrPoint v)
{
   Identity();

   double vx = v.X();
   double vy = v.Y();
   double vz = v.Z();

   double l = v.Length3();
   double d = sqrt(vy * vy + vz * vz);

   // We do the rotation around the vector setup, but
   // we make sure we're not close to the X axis.  If
   // we are not, we rotation onto the X axis to do
   // the rotation.  Otherwise, we rotation onto the 
   // Y axis. 

   if(d > 0.707 * l)
   {
      // Inverse rotation around X
      RotateX(vz / d, -vy / d);

      // Invese rotation around Y
      CGrTransform a;
      a.RotateY(vx/l, -d/l);
      Compose(a);

      // Rotation around X by the angle
      a.RotateX(r);
      Compose(a);

      // Rotation around Y
      a.RotateY(vx/l, d/l);
      Compose(a);

      // Rotation around X
      a.RotateX(vz / d, vy / d);
      Compose(a);
   }
   else
   {
      // Alternative is to rotate around the X axis
      // via alternative route.  We rotate onto
      // the XY plane, then down onto the X axis.

      d = sqrt(vx * vx + vz * vz);

      // Inverse rotation around Y
      RotateY(vx / d, -vz / d);

      // Invese rotation around Y
      CGrTransform a;
      a.RotateZ(d/l, vy/l);
      Compose(a);

      // Rotation around X by the angle
      a.RotateX(r);
      Compose(a);

      // Rotation around Z
      a.RotateZ(d/l, -vy/l);
      Compose(a);

      // Rotation around Y
      a.RotateY(vx / d, vz / d);
      Compose(a);
   }

   return *this;
}



CGrTransform & CGrTransform::Rotate(const CGrPoint &x, const CGrPoint &y, const CGrPoint &z)
{
   m[0][0] = x.X();  m[0][1] = x.Y();  m[0][2] = x.Z();  m[0][3] = 0;
   m[1][0] = y.X();  m[1][1] = y.Y();  m[1][2] = y.Z();  m[1][3] = 0;
   m[2][0] = z.X();  m[2][1] = z.Y();  m[2][2] = z.Z();  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

inline void _swap(double &a, double &b)
{
   double t = a;
   a = b;
   b = t;
}

CGrTransform & CGrTransform::Transpose()
{
   _swap(m[0][1], m[1][0]);
   _swap(m[0][2], m[2][0]);
   _swap(m[0][3], m[3][0]);
   _swap(m[1][2], m[2][1]);
   _swap(m[1][3], m[3][1]);
   _swap(m[2][3], m[3][2]);

   return *this;
}

//
// Name :         CGrTransform::LookAt() 
// Description :  Construct a LookAt matrix.  This is a rotation and
//                translation that will put the eye at 0,0,0, up in the
//                direction of the Y axis and looking down the -Z axis.
//

void CGrTransform::LookAt(double ex, double ey, double ez, 
                          double cx, double cy, double cz, 
                          double ux, double uy, double uz)
{
   CGrPoint eye(ex, ey, ez);
   CGrPoint center(cx, cy, cz);
   CGrPoint up(ux, uy, uz);
   
   CGrPoint cameraz = Normalize3(eye - center);
   CGrPoint camerax = Normalize3(Cross3(up, cameraz));
   CGrPoint cameray = Cross3(cameraz, camerax);

   CGrTransform r;
   r[0][0] = camerax.X();  r[0][1]= camerax.Y();  r[0][2] = camerax.Z();  r[0][3] = 0;
   r[1][0] = cameray.X();  r[1][1] = cameray.Y();  r[1][2] = cameray.Z();  r[1][3] = 0;
   r[2][0] = cameraz.X();  r[2][1] = cameraz.Y();  r[2][2] = cameraz.Z();  r[2][3] = 0;
   r[3][0] = r[3][1] = r[3][2] = 0.;  r[3][3] = 1.0;

   CGrTransform t;
   t.Translate(-ex, -ey, -ez);

   *this = r * t;
}
