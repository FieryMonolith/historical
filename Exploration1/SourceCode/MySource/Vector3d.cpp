/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#include <cmath>

#include "Vector3d.h"

double &Vector3d::at(int index) {
  if(index > 2) {
    index %= 3;
  } else if(index < 0) {
    index *= -1;
    index %= 3;
    if(index == 1) {
      index = 2;
    } else if(index == 2) {
      index = 1;
    }
  }

  return data[index];
}

const double &Vector3d::at(int index) const {
  if(index > 2) {
    index %= 3;
  } else if(index < 0) {
    index *= -1;
    index %= 3;
    if(index == 1) {
      index = 2;
    } else if(index == 2) {
      index = 1;
    }
  }

  return data[index];
}

Vector3d &Vector3d::operator=(const Vector3d &v) {
  data[0] = v.data[0];
  data[1] = v.data[1];
  data[2] = v.data[2];

  return *this;
}

Vector3d &Vector3d::operator+=(const Vector3d &v) {
  data[0] += v.data[0];
  data[1] += v.data[1];
  data[2] += v.data[2];

  return *this;
}

Vector3d &Vector3d::operator-=(const Vector3d &v) {
  data[0] -= v.data[0];
  data[1] -= v.data[1];
  data[2] -= v.data[2];

  return *this;
}

Vector3d &Vector3d::operator*=(double c) {
  data[0] *= c;
  data[1] *= c;
  data[2] *= c;

  return *this;
}

Vector3d &Vector3d::operator/=(double c) {
  if(c == 0) {
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
  } else {
    data[0] /= c;
    data[1] /= c;
    data[2] /= c;
  }

  return *this;
}

Vector3d operator+(const Vector3d &lhs, const Vector3d &rhs){
  return Vector3d(lhs[0]+rhs[0], lhs[1]+rhs[1], lhs[2]+rhs[2]);
}

Vector3d operator-(const Vector3d &lhs, const Vector3d &rhs){
  return Vector3d(lhs[0]-rhs[0], lhs[1]-rhs[1], lhs[2]-rhs[2]);
}

Vector3d operator-(const Vector3d &v){
  return Vector3d(-v[0], -v[1], -v[2]);
}

Vector3d operator*(double c, const Vector3d &v){
  return(Vector3d(c*v[0], c*v[1], c*v[2]));
}

Vector3d operator*(const Vector3d &v, double c){
  return(Vector3d(c*v[0], c*v[1], c*v[2]));
}

Vector3d operator/(const Vector3d &v, double c){
  return((c==0)?(Vector3d(0,0,0)):(Vector3d(v[0]/c, v[1]/c, v[2]/c)));
}

Vector3d Cross(const Vector3d &lhs, const Vector3d &rhs){
  return(Vector3d(
    lhs[1]*rhs[2] - lhs[2]*rhs[1],
    lhs[2]*rhs[0] - lhs[0]*rhs[2],
    lhs[0]*rhs[1] - lhs[1]*rhs[0]
  ));
}

double Inner(const Vector3d &lhs, const Vector3d &rhs) {
  return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
}

double Length(const Vector3d &v){
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

double Normalize(Vector3d &v) {
  double magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

  if(magnitude == 0) {
    v[0] = v[1] = v[2] = 0;
    return 0;
  }

  v /= magnitude;

#if 0
  if(isnan(v[0]) || isnan(v[1]) || isnan(v[2])) {
    v[0] = v[1] = v[2] = 0;
    return 0;
  }
#endif

  return magnitude;
}

double Distance(const Vector3d &v1, const Vector3d &v2) {
  double difference[3] = {v1[0]-v2[0], 
			  v1[1]-v2[1], 
			  v1[2]-v2[2]};
  double squareOfDifference[3] = {difference[0]*difference[0], 
				  difference[1]*difference[1],
				  difference[2]*difference[2]};
  double sumOfSquares = (squareOfDifference[0]+squareOfDifference[1]+
			 squareOfDifference[2]);

  return sqrt(sumOfSquares);
}

double DistanceSquared(const Vector3d &v1, const Vector3d &v2) {
  double difference[3] = {v1[0]-v2[0], 
			  v1[1]-v2[1], 
			  v1[2]-v2[2]};
  double squareOfDifference[3] = {difference[0]*difference[0], 
				  difference[1]*difference[1],
				  difference[2]*difference[2]};
  return squareOfDifference[0]+squareOfDifference[1]+squareOfDifference[2];
}

int operator==(const Vector3d &v1, const Vector3d &v2) {
  if(v1.data[0] == v2.data[0] && 
     v1.data[1] == v2.data[1] && 
     v1.data[2] == v2.data[2]) {
    return 1;
  }

  return 0;
}

int operator!=(const Vector3d &v1, const Vector3d &v2) {
  if(v1.data[0] != v2.data[0] || 
     v1.data[1] != v2.data[1] ||
     v1.data[2] != v2.data[2]) {
    return 1;
  }

  return 0;
}
