/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#include <cmath>

#include "Vector3d.h"
#include "Matrix.h"

Matrix3x3d::Matrix3x3d(void) {
  Identity();

  return;
}

Matrix3x3d::Matrix3x3d(const Matrix3x3d &m) {
  d_data[0][0] = m.d_data[0][0];
  d_data[0][1] = m.d_data[0][1];
  d_data[0][2] = m.d_data[0][2];
  d_data[1][0] = m.d_data[1][0];
  d_data[1][1] = m.d_data[1][1];
  d_data[1][2] = m.d_data[1][2];
  d_data[2][0] = m.d_data[2][0];
  d_data[2][1] = m.d_data[2][1];
  d_data[2][2] = m.d_data[2][2];

  return;
}

Matrix3x3d::Matrix3x3d(double v00, double v01, double v02, 
		                   double v10, double v11, double v12,
		                   double v20, double v21, double v22) {
  d_data[0][0] = v00;
  d_data[0][1] = v01;
  d_data[0][2] = v02;
  d_data[1][0] = v10;
  d_data[1][1] = v11;
  d_data[1][2] = v12;
  d_data[2][0] = v20;
  d_data[2][1] = v21;
  d_data[2][2] = v22;

  return;
}

Matrix3x3d::Matrix3x3d(const Vector3d &row0,
                       const Vector3d &row1, 
		                   const Vector3d &row2)
{
  d_data[0][0] = row0[0];
  d_data[0][1] = row0[1];
  d_data[0][2] = row0[2];
  d_data[1][0] = row1[0];
  d_data[1][1] = row1[1];
  d_data[1][2] = row1[2];
  d_data[2][0] = row2[0];
  d_data[2][1] = row2[1];
  d_data[2][2] = row2[2];

  return;
}

Matrix3x3d::~Matrix3x3d(void) {
  return;
}

Matrix3x3d &Matrix3x3d::operator=(const Matrix3x3d &m) {
  d_data[0][0] = m.d_data[0][0];
  d_data[0][1] = m.d_data[0][1];
  d_data[0][2] = m.d_data[0][2];
  d_data[1][0] = m.d_data[1][0];
  d_data[1][1] = m.d_data[1][1];
  d_data[1][2] = m.d_data[1][2];
  d_data[2][0] = m.d_data[2][0];
  d_data[2][1] = m.d_data[2][1];
  d_data[2][2] = m.d_data[2][2];

  return *this;
}

void Matrix3x3d::Zero(void) {
  d_data[0][0] = d_data[0][1] = d_data[0][2] = 0;
  d_data[1][0] = d_data[1][1] = d_data[1][2] = 0;
  d_data[2][0] = d_data[2][1] = d_data[2][2] = 0;

  return;
}

void Matrix3x3d::Identity(void) {
  d_data[0][0] = d_data[1][1] = d_data[2][2] = 1;

  d_data[0][1] = d_data[0][2] = d_data[1][0] = 0;
  d_data[1][2] = d_data[2][0] = d_data[2][1] = 0;

  return;
}

Matrix3x3d &Matrix3x3d::operator+=(const Matrix3x3d &m) {
  d_data[0][0] += m.d_data[0][0];
  d_data[0][1] += m.d_data[0][1];
  d_data[0][2] += m.d_data[0][2];
  d_data[1][0] += m.d_data[1][0];
  d_data[1][1] += m.d_data[1][1];
  d_data[1][2] += m.d_data[1][2];
  d_data[2][0] += m.d_data[2][0];
  d_data[2][1] += m.d_data[2][1];
  d_data[2][2] += m.d_data[2][2];

  return *this;
}

Matrix3x3d &Matrix3x3d::operator-=(const Matrix3x3d &m) {
  d_data[0][0] -= m.d_data[0][0];
  d_data[0][1] -= m.d_data[0][1];
  d_data[0][2] -= m.d_data[0][2];
  d_data[1][0] -= m.d_data[1][0];
  d_data[1][1] -= m.d_data[1][1];
  d_data[1][2] -= m.d_data[1][2];
  d_data[2][0] -= m.d_data[2][0];
  d_data[2][1] -= m.d_data[2][1];
  d_data[2][2] -= m.d_data[2][2];

  return *this;
}

Matrix3x3d &Matrix3x3d::operator*=(double c) {
  d_data[0][0] *= c;
  d_data[0][1] *= c;
  d_data[0][2] *= c;
  d_data[1][0] *= c;
  d_data[1][1] *= c;
  d_data[1][2] *= c;
  d_data[2][0] *= c;
  d_data[2][1] *= c;
  d_data[2][2] *= c;

  return *this;
}

Matrix3x3d &Matrix3x3d::operator/=(double c) {
  d_data[0][0] /= c;
  d_data[0][1] /= c;
  d_data[0][2] /= c;
  d_data[1][0] /= c;
  d_data[1][1] /= c;
  d_data[1][2] /= c;
  d_data[2][0] /= c;
  d_data[2][1] /= c;
  d_data[2][2] /= c;

  return *this;
}

Matrix3x3d operator*(double c, const Matrix3x3d &m){
  return Matrix3x3d(c*m.d_data[0][0], c*m.d_data[0][1], c*m.d_data[0][2],
		                c*m.d_data[1][0], c*m.d_data[1][1], c*m.d_data[1][2],
		                c*m.d_data[2][0], c*m.d_data[2][1], c*m.d_data[2][2]);
}

Matrix3x3d operator+(const Matrix3x3d &lhs, const Matrix3x3d &rhs){
  return Matrix3x3d(lhs.d_data[0][0] + rhs.d_data[0][0],
            		    lhs.d_data[0][1] + rhs.d_data[0][1],
		                lhs.d_data[0][2] + rhs.d_data[0][2],
		                lhs.d_data[1][0] + rhs.d_data[1][0],
		                lhs.d_data[1][1] + rhs.d_data[1][1],
		                lhs.d_data[1][2] + rhs.d_data[1][2],
		                lhs.d_data[2][0] + rhs.d_data[2][0],
		                lhs.d_data[2][1] + rhs.d_data[2][1],
		                lhs.d_data[2][2] + rhs.d_data[2][2]);
}

Matrix3x3d operator-(const Matrix3x3d &m){
  return (-1.0*m);
}

Matrix3x3d operator-(const Matrix3x3d &lhs, const Matrix3x3d &rhs){
  return Matrix3x3d(lhs.d_data[0][0] - rhs.d_data[0][0],
            		    lhs.d_data[0][1] - rhs.d_data[0][1],
		                lhs.d_data[0][2] - rhs.d_data[0][2],
            		    lhs.d_data[1][0] - rhs.d_data[1][0],
		                lhs.d_data[1][1] - rhs.d_data[1][1],
		                lhs.d_data[1][2] - rhs.d_data[1][2],
		                lhs.d_data[2][0] - rhs.d_data[2][0],
		                lhs.d_data[2][1] - rhs.d_data[2][1],
		                lhs.d_data[2][2] - rhs.d_data[2][2]);
}

Matrix3x3d operator*(const Matrix3x3d &lhs, const Matrix3x3d &rhs){
  return Matrix3x3d(lhs.d_data[0][0]*rhs.d_data[0][0] + 
		    lhs.d_data[0][1]*rhs.d_data[1][0] + 
		    lhs.d_data[0][2]*rhs.d_data[2][0],

		    lhs.d_data[0][0]*rhs.d_data[0][1] +
		    lhs.d_data[0][1]*rhs.d_data[1][1] + 
		    lhs.d_data[0][2]*rhs.d_data[2][1],

		    lhs.d_data[0][0]*rhs.d_data[0][2] +
		    lhs.d_data[0][1]*rhs.d_data[1][2] + 
		    lhs.d_data[0][2]*rhs.d_data[2][2],

		    lhs.d_data[1][0]*rhs.d_data[0][0] +
		    lhs.d_data[1][1]*rhs.d_data[1][0] + 
		    lhs.d_data[1][2]*rhs.d_data[2][0],

		    lhs.d_data[1][0]*rhs.d_data[0][1] +
		    lhs.d_data[1][1]*rhs.d_data[1][1] + 
		    lhs.d_data[1][2]*rhs.d_data[2][1],

		    lhs.d_data[1][0]*rhs.d_data[0][2] +
		    lhs.d_data[1][1]*rhs.d_data[1][2] + 
		    lhs.d_data[1][2]*rhs.d_data[2][2],

		    lhs.d_data[2][0]*rhs.d_data[0][0] +
		    lhs.d_data[2][1]*rhs.d_data[1][0] + 
		    lhs.d_data[2][2]*rhs.d_data[2][0],

		    lhs.d_data[2][0]*rhs.d_data[0][1] +
		    lhs.d_data[2][1]*rhs.d_data[1][1] + 
		    lhs.d_data[2][2]*rhs.d_data[2][1],

		    lhs.d_data[2][0]*rhs.d_data[0][2] +
		    lhs.d_data[2][1]*rhs.d_data[1][2] + 
		    lhs.d_data[2][2]*rhs.d_data[2][2]);
}

Matrix3x3d Transpose(const Matrix3x3d &m){
  return Matrix3x3d(m.d_data[0][0], m.d_data[1][0], m.d_data[2][0],
		                m.d_data[0][1], m.d_data[1][1], m.d_data[2][1],
		                m.d_data[0][2], m.d_data[1][2], m.d_data[2][2]);
}

int operator==(const Matrix3x3d &m1, const Matrix3x3d &m2) {
  if(m1.d_data[0][0] == m2.d_data[0][0] &&
     m1.d_data[1][0] == m2.d_data[1][0] &&
     m1.d_data[2][0] == m2.d_data[2][0] &&
     m1.d_data[0][1] == m2.d_data[0][1] &&
     m1.d_data[1][1] == m2.d_data[1][1] &&
     m1.d_data[2][1] == m2.d_data[2][1] &&
     m1.d_data[0][2] == m2.d_data[0][2] &&
     m1.d_data[1][2] == m2.d_data[1][2] &&
     m1.d_data[2][2] == m2.d_data[2][2]) {
    return 1;
  }

  return 0;
}

int operator!=(const Matrix3x3d &m1, const Matrix3x3d &m2) {
  if(m1.d_data[0][0] != m2.d_data[0][0] ||
     m1.d_data[1][0] != m2.d_data[1][0] ||
     m1.d_data[2][0] != m2.d_data[2][0] ||
     m1.d_data[0][1] != m2.d_data[0][1] ||
     m1.d_data[1][1] != m2.d_data[1][1] ||
     m1.d_data[2][1] != m2.d_data[2][1] ||
     m1.d_data[0][2] != m2.d_data[0][2] ||
     m1.d_data[1][2] != m2.d_data[1][2] ||
     m1.d_data[2][2] != m2.d_data[2][2]) {
    return 1;
  }

  return 0;
}

Matrix4x4d::Matrix4x4d(void) {
  Identity();

  return;
}

Matrix4x4d operator*(const Matrix4x4d &lhs, const Matrix4x4d &rhs){
  return Matrix4x4d(lhs.d_data[0][0]*rhs.d_data[0][0] + 
		    lhs.d_data[0][1]*rhs.d_data[1][0] + 
		    lhs.d_data[0][2]*rhs.d_data[2][0] + 
		    lhs.d_data[0][3]*rhs.d_data[3][0],

		    lhs.d_data[0][0]*rhs.d_data[0][1] +
		    lhs.d_data[0][1]*rhs.d_data[1][1] + 
		    lhs.d_data[0][2]*rhs.d_data[2][1] + 
		    lhs.d_data[0][3]*rhs.d_data[3][1],

		    lhs.d_data[0][0]*rhs.d_data[0][2] +
		    lhs.d_data[0][1]*rhs.d_data[1][2] + 
		    lhs.d_data[0][2]*rhs.d_data[2][2] + 
		    lhs.d_data[0][3]*rhs.d_data[3][2],

		    lhs.d_data[0][0]*rhs.d_data[0][3] +
		    lhs.d_data[0][1]*rhs.d_data[1][3] + 
		    lhs.d_data[0][2]*rhs.d_data[2][3] + 
		    lhs.d_data[0][3]*rhs.d_data[3][3],

		    lhs.d_data[1][0]*rhs.d_data[0][0] +
		    lhs.d_data[1][1]*rhs.d_data[1][0] + 
		    lhs.d_data[1][2]*rhs.d_data[2][0] + 
		    lhs.d_data[1][3]*rhs.d_data[3][0],

		    lhs.d_data[1][0]*rhs.d_data[0][1] +
		    lhs.d_data[1][1]*rhs.d_data[1][1] + 
		    lhs.d_data[1][2]*rhs.d_data[2][1] + 
		    lhs.d_data[1][3]*rhs.d_data[3][1],

		    lhs.d_data[1][0]*rhs.d_data[0][2] +
		    lhs.d_data[1][1]*rhs.d_data[1][2] + 
		    lhs.d_data[1][2]*rhs.d_data[2][2] + 
		    lhs.d_data[1][3]*rhs.d_data[3][2],

		    lhs.d_data[1][0]*rhs.d_data[0][3] +
		    lhs.d_data[1][1]*rhs.d_data[1][3] + 
		    lhs.d_data[1][2]*rhs.d_data[2][3] + 
		    lhs.d_data[1][3]*rhs.d_data[3][3],

		    lhs.d_data[2][0]*rhs.d_data[0][0] +
		    lhs.d_data[2][1]*rhs.d_data[1][0] + 
		    lhs.d_data[2][2]*rhs.d_data[2][0] + 
		    lhs.d_data[2][3]*rhs.d_data[3][0],

		    lhs.d_data[2][0]*rhs.d_data[0][1] +
		    lhs.d_data[2][1]*rhs.d_data[1][1] + 
		    lhs.d_data[2][2]*rhs.d_data[2][1] + 
		    lhs.d_data[2][3]*rhs.d_data[3][1],

		    lhs.d_data[2][0]*rhs.d_data[0][2] +
		    lhs.d_data[2][1]*rhs.d_data[1][2] + 
		    lhs.d_data[2][2]*rhs.d_data[2][2] + 
		    lhs.d_data[2][3]*rhs.d_data[3][2],

		    lhs.d_data[2][0]*rhs.d_data[0][3] +
		    lhs.d_data[2][1]*rhs.d_data[1][3] + 
		    lhs.d_data[2][2]*rhs.d_data[2][3] + 
		    lhs.d_data[2][3]*rhs.d_data[3][3],

		    lhs.d_data[3][0]*rhs.d_data[0][0] +
		    lhs.d_data[3][1]*rhs.d_data[1][0] + 
		    lhs.d_data[3][2]*rhs.d_data[2][0] + 
		    lhs.d_data[3][3]*rhs.d_data[3][0],

		    lhs.d_data[3][0]*rhs.d_data[0][1] +
		    lhs.d_data[3][1]*rhs.d_data[1][1] + 
		    lhs.d_data[3][2]*rhs.d_data[2][1] + 
		    lhs.d_data[3][3]*rhs.d_data[3][1],

		    lhs.d_data[3][0]*rhs.d_data[0][2] +
		    lhs.d_data[3][1]*rhs.d_data[1][2] + 
		    lhs.d_data[3][2]*rhs.d_data[2][2] + 
		    lhs.d_data[3][3]*rhs.d_data[3][2],

		    lhs.d_data[3][0]*rhs.d_data[0][3] +
		    lhs.d_data[3][1]*rhs.d_data[1][3] + 
		    lhs.d_data[3][2]*rhs.d_data[2][3] + 
		    lhs.d_data[3][3]*rhs.d_data[3][3]);
}

Matrix4x4d Transpose(const Matrix4x4d &m){
  return Matrix4x4d(m.d_data[0][0], m.d_data[1][0], m.d_data[2][0], m.d_data[3][0],
		                m.d_data[0][1], m.d_data[1][1], m.d_data[2][1], m.d_data[3][1],
		                m.d_data[0][2], m.d_data[1][2], m.d_data[2][2], m.d_data[3][2],
		                m.d_data[0][3], m.d_data[1][3], m.d_data[2][3], m.d_data[3][3]);
}

void Matrix4x4d::Identity(void) {
  d_data[0][1] = d_data[0][2] = d_data[0][3] = 0.0;
  d_data[1][0] = d_data[1][2] = d_data[1][3] = 0.0;
  d_data[2][0] = d_data[2][1] = d_data[2][3] = 0.0;
  d_data[3][0] = d_data[3][1] = d_data[3][2] = 0.0;

  d_data[0][0] = d_data[1][1] = d_data[2][2] = d_data[3][3] = 1.0;

  return;
}

void Matrix4x4d::Zero(void) {
  d_data[0][1] = d_data[0][2] = d_data[0][3] = 0.0;
  d_data[1][0] = d_data[1][2] = d_data[1][3] = 0.0;
  d_data[2][0] = d_data[2][1] = d_data[2][3] = 0.0;
  d_data[3][0] = d_data[3][1] = d_data[3][2] = 0.0;

  d_data[0][0] = d_data[1][1] = d_data[2][2] = d_data[3][3] = 0.0;

  return;
}

int operator==(const Matrix4x4d &m1, const Matrix4x4d &m2) {
  if(m1.d_data[0][0] == m2.d_data[0][0] &&
     m1.d_data[1][0] == m2.d_data[1][0] &&
     m1.d_data[2][0] == m2.d_data[2][0] &&
     m1.d_data[3][0] == m2.d_data[3][0] &&
     m1.d_data[0][1] == m2.d_data[0][1] &&
     m1.d_data[1][1] == m2.d_data[1][1] &&
     m1.d_data[2][1] == m2.d_data[2][1] &&
     m1.d_data[3][1] == m2.d_data[3][1] &&
     m1.d_data[0][2] == m2.d_data[0][2] &&
     m1.d_data[1][2] == m2.d_data[1][2] &&
     m1.d_data[2][2] == m2.d_data[2][2] &&
     m1.d_data[3][2] == m2.d_data[3][2] &&
     m1.d_data[0][3] == m2.d_data[0][3] &&
     m1.d_data[1][3] == m2.d_data[1][3] &&
     m1.d_data[2][3] == m2.d_data[2][3] &&
     m1.d_data[3][3] == m2.d_data[3][3]) {
    return 1;
  }

  return 0;
}

int operator!=(const Matrix4x4d &m1, const Matrix4x4d &m2) {
  if(m1.d_data[0][0] != m2.d_data[0][0] ||
     m1.d_data[1][0] != m2.d_data[1][0] ||
     m1.d_data[2][0] != m2.d_data[2][0] ||
     m1.d_data[3][0] != m2.d_data[3][0] ||
     m1.d_data[0][1] != m2.d_data[0][1] ||
     m1.d_data[1][1] != m2.d_data[1][1] ||
     m1.d_data[2][1] != m2.d_data[2][1] ||
     m1.d_data[3][1] != m2.d_data[3][1] ||
     m1.d_data[0][2] != m2.d_data[0][2] ||
     m1.d_data[1][2] != m2.d_data[1][2] ||
     m1.d_data[2][2] != m2.d_data[2][2] ||
     m1.d_data[3][2] != m2.d_data[3][2] ||
     m1.d_data[0][3] != m2.d_data[0][3] ||
     m1.d_data[1][3] != m2.d_data[1][3] ||
     m1.d_data[2][3] != m2.d_data[2][3] ||
     m1.d_data[3][3] != m2.d_data[3][3]) {
    return 1;
  }

  return 0;
}
