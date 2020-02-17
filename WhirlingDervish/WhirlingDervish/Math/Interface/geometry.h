#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Vector3d;

// u is a scalar
// n is the normal vector to the plane
// p2 and p1 define a line segment that may intersect the given plane
// v = p2 - p1
// p = p1 + (u * v)
//
// if Inner(n, v) == 0, then the line is parallel or on the plane.
// if the line is parallel the function returns false, otherwise true
// if the line is on the plane, then p2 is returned as the solution
//
// else
// p is the point of intersection
//
// u = Inner(normal, (pointOnPlane - p1)) / Inner(n, v)
bool IntersectionVectorPlane(const Vector3d &p1,
			     const Vector3d &p2,
			     const Vector3d &n,
			     const Vector3d &pointOnPlane,
			     Vector3d &intersectionPoint);

#if 0
// This isn't working yet, currently this is for a cylinder that is vertical
bool IntersectionVectorCylinder(Vector3d &out, 
				const Vector3d &p0, const Vector3d &p1, 
				const Vector3d &cylinderBottomOrigin, 
				const Vector3d &cylinderTopOrigin,
				double radius);
#endif

#if 0
bool PointInside(Vector3d *testPoint, long verticesCount, Vector3d **vertices);
#endif

#endif
