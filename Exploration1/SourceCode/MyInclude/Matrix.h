/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

class Vector3d;

class Matrix3x3d {
public:
  Matrix3x3d(void);
  Matrix3x3d(double v00, double v01, double v02, 
	     double v10, double v11, double v12,
	     double v20, double v21, double v22);
  Matrix3x3d(const Vector3d &row0, const Vector3d &row1, const Vector3d &row2);
  Matrix3x3d(const Matrix3x3d &);
  ~Matrix3x3d(void) { return; }

  Matrix3x3d &operator=(const Matrix3x3d &);

  const double *Array(void) const { return &(d_data[0][0]); }
  double &At(unsigned int i, unsigned int j);
  const double &At(unsigned int i, unsigned int j) const;

  void Zero(void);
  void Identity(void);

  Matrix3x3d &operator+=(const Matrix3x3d &);
  Matrix3x3d &operator-=(const Matrix3x3d &);
  Matrix3x3d &operator*=(double);
  Matrix3x3d &operator/=(double);

private:
  double d_data[3][3];

  friend Matrix3x3d operator*(double, const Matrix3x3d &);
  friend Matrix3x3d operator*(const Matrix3x3d &, double);
  friend Matrix3x3d operator+(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
  friend Matrix3x3d operator-(const Matrix3x3d &);
  friend Matrix3x3d operator-(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
  friend Matrix3x3d operator*(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
  friend Matrix3x3d Transpose(const Matrix3x3d &);
  friend int operator==(const Matrix3x3d &, const Matrix3x3d &);
  friend int operator!=(const Matrix3x3d &, const Matrix3x3d &);
  friend class Matrix4x4d;
};

Matrix3x3d operator*(double, const Matrix3x3d &);
Matrix3x3d operator*(const Matrix3x3d &, double);
Matrix3x3d operator+(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
Matrix3x3d operator-(const Matrix3x3d &);
Matrix3x3d operator-(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
Matrix3x3d operator*(const Matrix3x3d &lhs, const Matrix3x3d &rhs);
Matrix3x3d Transpose(const Matrix3x3d &);
int operator==(const Matrix3x3d &, const Matrix3x3d &);
int operator!=(const Matrix3x3d &, const Matrix3x3d &);

class Matrix4x4d {
public:
  Matrix4x4d(void);
  Matrix4x4d(double v00, double v01, double v02, double v03,
	     double v10, double v11, double v12, double v13,
             double v20, double v21, double v22, double v23,
	     double v30, double v31, double v32, double v33);
  Matrix4x4d(const Matrix3x3d &m, const Vector3d &p);
  Matrix4x4d(const Matrix4x4d &m);
  ~Matrix4x4d(void) { return; }

  Matrix4x4d &operator=(const Matrix4x4d &m);

  const double *Array(void) const { return &(d_data[0][0]); }
  double &At(unsigned int i, unsigned int j);
  const double &At(unsigned int i, unsigned int j) const;

  void Identity(void);
  void Zero(void);

  Matrix4x4d &operator+=(const Matrix4x4d &);
  Matrix4x4d &operator-=(const Matrix4x4d &);
  Matrix4x4d &operator*=(double);
  Matrix4x4d &operator/=(double);

private:
  double d_data[4][4];

  friend Matrix4x4d operator*(double, const Matrix4x4d &);
  friend Matrix4x4d operator*(const Matrix4x4d &, double);
  friend Matrix4x4d operator+(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
  friend Matrix4x4d operator-(const Matrix4x4d &);
  friend Matrix4x4d operator-(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
  friend Matrix4x4d operator*(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
  friend Matrix4x4d Transpose(const Matrix4x4d &);
  friend int operator==(const Matrix4x4d &, const Matrix4x4d &);
  friend int operator!=(const Matrix4x4d &, const Matrix4x4d &);
  friend class Matrix3x3d;
};

Matrix4x4d operator*(double, const Matrix4x4d &);
Matrix4x4d operator*(const Matrix4x4d &, double);
Matrix4x4d operator+(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
Matrix4x4d operator-(const Matrix4x4d &);
Matrix4x4d operator-(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
Matrix4x4d operator*(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
Matrix4x4d Transpose(const Matrix4x4d &);
int operator==(const Matrix4x4d &, const Matrix4x4d &);
int operator!=(const Matrix4x4d &, const Matrix4x4d &);

#endif
