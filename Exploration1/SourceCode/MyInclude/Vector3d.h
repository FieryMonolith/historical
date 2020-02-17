/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __JVECTOR_H__
#define __JVECTOR_H__

class Vector3d {
private:
  double data[3];

public:
  Vector3d(double a=0, double b=0, double c=0) { data[0] = a; data[1] = b; 
                                                 data[2] = c;
						 return;
                                               }
  Vector3d(const Vector3d &v) { data[0] = v.data[0]; data[1] = v.data[1];
                                data[2] = v.data[2];
				return;
                              }
  ~Vector3d(void) { return; }

  const double &at(int index) const;
  double &at(int index);
  const double &operator[](int index) const { return data[index]; }
  double &operator[](int index) { return data[index]; }
  const double *Array(void) const { return data; }

  Vector3d &operator=(const Vector3d &);
  Vector3d &operator+=(const Vector3d &);
  Vector3d &operator-=(const Vector3d &);
  Vector3d &operator*=(double);
  Vector3d &operator/=(double);

  void Zero(void) { data[0] = data[1] = data[2] = 0; return; }

private:
  friend Vector3d operator+(const Vector3d &lhs, const Vector3d &rhs);
  friend Vector3d operator-(const Vector3d &lhs, const Vector3d &rhs);
  friend Vector3d operator-(const Vector3d &);
  friend Vector3d operator*(double, const Vector3d &);
  friend Vector3d operator*(const Vector3d &, double);
  friend Vector3d operator/(const Vector3d &, double);

  friend Vector3d Cross(const Vector3d &lhs, const Vector3d &rhs);
  friend double Inner(const Vector3d &lhs, const Vector3d &rhs);
  friend double Length(const Vector3d &);
  friend double Normalize(Vector3d &);
  friend double Distance(const Vector3d &, const Vector3d &);
  friend double DistanceSquared(const Vector3d &, const Vector3d &);

  friend int operator==(const Vector3d &, const Vector3d &);
  friend int operator!=(const Vector3d &, const Vector3d &);
};

Vector3d operator+(const Vector3d &lhs, const Vector3d &rhs);
Vector3d operator-(const Vector3d &lhs, const Vector3d &rhs);
Vector3d operator-(const Vector3d &);
Vector3d operator*(double, const Vector3d &);
Vector3d operator*(const Vector3d &, double);
Vector3d operator/(const Vector3d &, double);

Vector3d Cross(const Vector3d &lhs, const Vector3d &rhs);
double Inner(const Vector3d &lhs, const Vector3d &rhs); // Dot Product
double Length(const Vector3d &);
double Normalize(Vector3d &);
double Distance(const Vector3d &, const Vector3d &);
double DistanceSquared(const Vector3d &, const Vector3d &);

int operator==(const Vector3d &, const Vector3d &);
int operator!=(const Vector3d &, const Vector3d &);

#endif
