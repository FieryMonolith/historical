#ifndef __JROTATION_H__
#define __JROTATION_H__

class Vector3d;
class Matrix4x4d;

Matrix4x4d RotationFromZ(const Vector3d &to);
Matrix4x4d RotationToZ(const Vector3d &from);
void CalculateRotationAnglesToZ(double &theta, double &phi, 
				const Vector3d &direction);
void Orthonormalize(Matrix4x4d &m);

#endif
