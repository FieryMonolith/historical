#include "jvector.h"
#include "matrix.h"

#include "geometry.h"

// See header
bool IntersectionVectorPlane(const Vector3d &p1, 
			     const Vector3d &p2,
			     const Vector3d &n,
			     const Vector3d &pointOnPlane,
			     Vector3d &intersectionPoint) {
  Vector3d v(p2 - p1);
  Vector3d v2(pointOnPlane - p1);
  double x = Inner(n, v);
  if(x == 0) {
    x = Inner(n, v2);
    if(x == 0) {
      intersectionPoint = p2;
      return true;
    }
    return false;
  }

  double u = Inner(n, v2) / x;
  intersectionPoint = p1 + u*v;

  return true;
}

#if 0
bool IntersectionVectorCylinder(Vector3d &out, 
				const Vector3d &p0, const Vector3d &p1, 
				const Vector3d &cylinderBottomOrigin, 
				const Vector3d &cylinderTopOrigin,
				double radius) {
  double radius = d_pillarRadius + 5.0;
  Vector3d pillar(*d_pillar1);
  pillar[1] = 0.0;
  Vector3d pos(p);
  pos[1] = 0.0;
  Vector3d o(origin);
  o[1] = 0.0;

  Vector3d vco(pillar - o);
  Vector3d vpo(pos - o);
  double doc = Normalize(vco);
  double dop = Normalize(vpo);
  if(doc - dop >= radius) {
    return;
  }

  double theta = asin(radius / doc);
  if(theta < 0) { theta *= -1.0; }
  double phi = Inner(vco, vpo);
  if(phi < 0) { phi *= -1.0; }
  if(phi >= theta) {
    return;
  }

  double a = doc * sin(phi);
  double u = sqrt((a*a) + (doc*doc));
  double v = sqrt((a*a) + (radius * radius));
  double alpha = u - v;

  Vector3d np(p - o);
  np[1] = 0.0;
  Normalize(np);
  pos = o + alpha * np;
  double run = sqrt((np[0] * np[0]) + (np[2] * np[2]));
  np = Vector3d(run, p[1]-origin[1], 0.0);
  Normalize(np);
  double rise = (alpha / run * np[1]);
  pos[1] = origin[1] + rise;
  double length = sqrt((rise * rise) + (alpha * alpha));
  dop = Distance(p, origin);
  if(pos[1] < 0 || pos[1] > d_pillarHeight || length >= dop) {
    return;
  }

  p = pos;

  return;
}
#endif

#if 0
bool PointInside(const Vector &testPoint, long verticesCount, 
		 Vector **vertices) {
  Vector *normals = new Vector[verticesCount];
  Vector testPointVertex, vertexNextVertex;

  for(long i=0; i<verticesCount; i++) {
    testPointVertex[0] = testPoint[0] - vertices[i][0];
    testPointVertex[0] = testPoint->v2 - vertices[i]->v2;
    testPointVertex[0] = testPoint->v3 - vertices[i]->v3;

    vertexNextVertex[0] = vertices[(i+1)%verticesCount][0] - vertices[i][0];
    vertexNextVertex[0] = vertices[(i+1)%verticesCount][0] - vertices[i][0];
    vertexNextVertex[0] = vertices[(i+1)%verticesCount][0] - vertices[i][0];

    normals[i][0] = (testPointVertex[1]*vertexNextVertex[2])-
                    (testPointVertex[2]*vertexNextVertex[1]);
    normals[i][1] = (testPointVertex[2]*vertexNextVertex[0])-
                    (testPointVertex[0]*vertexNextVertex[2]);
    normals[i][2] = (testPointVertex[0]*vertexNextVertex[1])-
                    (testPointVertex[1]*vertexNextVertex[0]);
  }

  for(long i=0; i<verticesCount; i++) {
    if(!(((normals[i][0] < -0.0000000001 && normals[i+1][0] < -0.0000000001) ||
	  (normals[i][0] > 0.0000000001 && normals[i+1][0] > 0.0000000001) ||
	  (fabs(normals[i][0] - normals[i+1][0]) < 0.000000001)) &&
	 ((normals[i][1] < -0.0000000001 && normals[i+1][1] < -0.0000000001) ||
	  (normals[i][1] > 0.0000000001 && normals[i+1][1] > 0.0000000001) ||
	  (fabs(normals[i][1] - normals[i+1][1]) < 0.000000001)) &&
	 ((normals[i][2] < -0.0000000001 && normals[i+1][2] < -0.0000000001) ||
	  (normals[i][2] > 0.0000000001 && normals[i+1][2] > 0.0000000001) ||
	  (fabs(normals[i][2] - normals[i+1][2]) < 0.000000001)))) {
      delete normals;
      return false;
    }
  }

  delete normals;

  return true;
}
#endif
