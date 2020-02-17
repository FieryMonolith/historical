//
// Name :         GrTransform.h
// Description :  Header file for CGrTransform.  This class implements
//                a 4x4 transformation matrix.  
//

#if !defined(AFX_GRTRANSFORM_H__BB2CDF1C_D660_4826_AD0E_16C7D5C04A29__INCLUDED_)
#define AFX_GRTRANSFORM_H__BB2CDF1C_D660_4826_AD0E_16C7D5C04A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrPoint.h"
#include "GL/gl.h"

const double GR_PI = 3.1415926535897932384626433832795;
const double GR_PI2 = 2. * GR_PI;
const double GR_RTOD = 180. / GR_PI;      // Converts radians to degrees
const double GR_DTOR = GR_PI / 180.;      // Converts degrees to radians

class CGrTransform  
{
public:
	void LookAt(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz);
   CGrTransform() {}
   ~CGrTransform() {}

	void Zero();
	void Identity();

   CGrTransform &Translate(double x, double y, double z) {Identity(); m[0][3]=x; m[1][3]=y; m[2][3]=z;  return *this;}
   CGrTransform &Translate(const CGrPoint &p) {Identity(); m[0][3]=p.X(); m[1][3]=p.Y(); m[2][3]=p.Z(); return *this;}
   CGrTransform &Rotate(double r, const CGrPoint v);
	CGrTransform &RotateX(double r);
	CGrTransform &RotateX(double cr, double sr);
	CGrTransform &RotateY(double r);
	CGrTransform &RotateY(double cr, double sr);
	CGrTransform &RotateZ(double r);
	CGrTransform &RotateZ(double cr, double sr);
	CGrTransform & Rotate(const CGrPoint &x, const CGrPoint &y, const CGrPoint &z);
	CGrTransform & Transpose();

   CGrTransform &Compose(const CGrTransform &b);
   CGrTransform &operator*=(const CGrTransform &b) {return Compose(b);}

   double *operator[](int r) {return m[r];}
   const double * operator[](int r) const {return m[r];}

   void glMultMatrix() const
   {
      double mm[16];
      for(int i=0;  i<4;  i++)
         for(int j=0;  j<4;  j++)
         {
            mm[i * 4 + j] = m[j][i];
         }

      ::glMultMatrixd(mm);
   }

private:
   double m[4][4];
};

inline CGrTransform operator *(const CGrTransform &a, const CGrTransform &b)
{
   CGrTransform x;
   for(int r=0;  r<4;  r++)
      for(int c=0;  c<4;  c++)
      {
         x[r][c] = a[r][0] * b[0][c] + a[r][1] * b[1][c] + a[r][2] * b[2][c] + a[r][3] * b[3][c];
      }

   return x;
}

inline CGrTransform &CGrTransform::Compose(const CGrTransform &b)
{
   *this = *this * b;
   return *this;
}

inline CGrPoint operator *(const CGrTransform &a, const CGrPoint &p)
{
   return CGrPoint(a[0][0] * p.X() + a[0][1] * p.Y() + a[0][2] * p.Z() + a[0][3] * p.W(),
               a[1][0] * p.X() + a[1][1] * p.Y() + a[1][2] * p.Z() + a[1][3] * p.W(),
               a[2][0] * p.X() + a[2][1] * p.Y() + a[2][2] * p.Z() + a[2][3] * p.W(),
               a[3][0] * p.X() + a[3][1] * p.Y() + a[3][2] * p.Z() + a[3][3] * p.W());
}

inline CGrTransform Transpose(const CGrTransform &t)
{
   CGrTransform r(t);
   return r.Transpose();
}

#endif // !defined(AFX_GRTRANSFORM_H__BB2CDF1C_D660_4826_AD0E_16C7D5C04A29__INCLUDED_)
