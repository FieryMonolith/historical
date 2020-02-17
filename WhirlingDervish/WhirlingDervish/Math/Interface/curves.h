#ifndef __JCURVES_H__
#define __JCURVES_H__

class Vector3d;

class JCurves {
public:
  static bool CatmullRomPosition(Vector3d &out, double t, const Vector3d &p0, 
				 const Vector3d &p1, const Vector3d &p2, 
				 const Vector3d &p3);
  static bool CatmullRomTangent(Vector3d &out, double t, const Vector3d &p0, 
				const Vector3d &p1, const Vector3d &p2, 
				const Vector3d &p3);
  static bool FergusonPosition(Vector3d &out, double t, const Vector3d &p0, 
			       const Vector3d &p1, const Vector3d &tangent0, 
			       const Vector3d &tangent1);
  static bool FergusonTangent(Vector3d &out, double t, const Vector3d &p0, 
			      const Vector3d &p1, const Vector3d &tangent0, 
			      const Vector3d &tangent1);

private:
  JCurves(void);
  ~JCurves(void);
  JCurves(const JCurves &);
};

#endif
