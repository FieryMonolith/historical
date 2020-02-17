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
  Matrix3x3d(const Matrix3x3d &);
  Matrix3x3d(double v00, double v01, double v02, 
	           double v10, double v11, double v12,
	           double v20, double v21, double v22);
  Matrix3x3d(const Vector3d &row0, const Vector3d &row1, const Vector3d &row2);
  ~Matrix3x3d(void);

  Matrix3x3d &operator=(const Matrix3x3d &);

  const double *Array(void) const { return &(d_data[0][0]); }
  double &At(unsigned int i, unsigned int j) { return d_data[i][j]; }
  const double &At(unsigned int i, unsigned int j) const {return d_data[i][j];}

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
Matrix3x3d operator*(const Matrix3x3d &, const Vector3d &);
Matrix3x3d Transpose(const Matrix3x3d &);
int operator==(const Matrix3x3d &, const Matrix3x3d &);
int operator!=(const Matrix3x3d &, const Matrix3x3d &);

class Matrix4x4d {
public:
  Matrix4x4d(void);
  Matrix4x4d(double v00, double v01, double v02, double v03,
	           double v10, double v11, double v12, double v13,
             double v20, double v21, double v22, double v23,
	           double v30, double v31, double v32, double v33) {
    d_data[0][0] = v00;
    d_data[0][1] = v01;
    d_data[0][2] = v02;
    d_data[0][3] = v03;
    d_data[1][0] = v10;
    d_data[1][1] = v11;
    d_data[1][2] = v12;
    d_data[1][3] = v13;
    d_data[2][0] = v20;
    d_data[2][1] = v21;
    d_data[2][2] = v22;
    d_data[2][3] = v23;
    d_data[3][0] = v30;
    d_data[3][1] = v31;
    d_data[3][2] = v32;
    d_data[3][3] = v33;

    return;
  }

  Matrix4x4d::Matrix4x4d(const Matrix3x3d &m, const Vector3d &p) {
    d_data[0][0] = m.d_data[0][0];
    d_data[0][1] = m.d_data[0][1];
    d_data[0][2] = m.d_data[0][2];
    d_data[0][3] = p[0];
    d_data[1][0] = m.d_data[1][0];
    d_data[1][1] = m.d_data[1][1];
    d_data[1][2] = m.d_data[1][2];
    d_data[1][3] = p[1];
    d_data[2][0] = m.d_data[2][0];
    d_data[2][1] = m.d_data[2][1];
    d_data[2][2] = m.d_data[2][2];
    d_data[2][3] = p[2];
    d_data[3][0] = 0.0;
    d_data[3][1] = 0.0;
    d_data[3][2] = 0.0;
    d_data[3][3] = 1.0;

    return;
  }

  Matrix4x4d(const Matrix4x4d &m) {
    d_data[0][0] = m.d_data[0][0];
    d_data[0][1] = m.d_data[0][1];
    d_data[0][2] = m.d_data[0][2];
    d_data[0][3] = m.d_data[0][3];
    d_data[1][0] = m.d_data[1][0];
    d_data[1][1] = m.d_data[1][1];
    d_data[1][2] = m.d_data[1][2];
    d_data[1][3] = m.d_data[1][3];
    d_data[2][0] = m.d_data[2][0];
    d_data[2][1] = m.d_data[2][1];
    d_data[2][2] = m.d_data[2][2];
    d_data[2][3] = m.d_data[2][3];
    d_data[3][0] = m.d_data[3][0];
    d_data[3][1] = m.d_data[3][1];
    d_data[3][2] = m.d_data[3][2];
    d_data[3][3] = m.d_data[3][3];

    return;
  }
  ~Matrix4x4d(void) { return; }

  Matrix4x4d &operator=(const Matrix4x4d &m) {
    d_data[0][0] = m.d_data[0][0];
    d_data[0][1] = m.d_data[0][1];
    d_data[0][2] = m.d_data[0][2];
    d_data[0][3] = m.d_data[0][3];
    d_data[1][0] = m.d_data[1][0];
    d_data[1][1] = m.d_data[1][1];
    d_data[1][2] = m.d_data[1][2];
    d_data[1][3] = m.d_data[1][3];
    d_data[2][0] = m.d_data[2][0];
    d_data[2][1] = m.d_data[2][1];
    d_data[2][2] = m.d_data[2][2];
    d_data[2][3] = m.d_data[2][3];
    d_data[3][0] = m.d_data[3][0];
    d_data[3][1] = m.d_data[3][1];
    d_data[3][2] = m.d_data[3][2];
    d_data[3][3] = m.d_data[3][3];

    return *this;
  }

  const double *Array(void) const { return &(d_data[0][0]); }
  double &At(unsigned int i, unsigned int j) { return d_data[i][j]; }
  const double &At(unsigned int i, unsigned int j) const {return d_data[i][j];}
  void Identity(void);
  void Zero(void);

private:
  double d_data[4][4];

  friend Matrix4x4d operator*(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
  friend Matrix4x4d Transpose(const Matrix4x4d &);
  friend class Matrix3x3d;
  friend int operator==(const Matrix4x4d &, const Matrix4x4d &);
  friend int operator!=(const Matrix4x4d &, const Matrix4x4d &);
};

Matrix4x4d operator*(const Matrix4x4d &lhs, const Matrix4x4d &rhs);
Matrix4x4d Transpose(const Matrix4x4d &);
int operator==(const Matrix4x4d &, const Matrix4x4d &);
int operator!=(const Matrix4x4d &, const Matrix4x4d &);

#endif
