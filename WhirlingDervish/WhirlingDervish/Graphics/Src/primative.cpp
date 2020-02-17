#include <iostream>
#include <cassert>

#include "primative.h"

using namespace std;

Primative::Primative(void) {
  d_valid = false;

  return;
}

Primative::~Primative(void) {
  Clear();

  return;
}

bool Primative::Initialize(const QString &data) {
  d_valid = false;
  if(data.isNull() || data.isEmpty()) {
    return false;
  }

  std::map<int, int> pointIdToIndex;
  bool valid;
  int integer = 0;
  double f = 0.0;
  QString value;

  value = data.section(':', 0, 0);
  integer = value.toUInt(&valid);
  if(!valid) { return false; }
  d_creatureId = integer;

  value = data.section(':', 1, 1);
  integer = value.toInt(&valid);
  if(!valid) { return false; }
  d_pointCount = integer;

  value = data.section(':', 2, 2);
  integer = value.toInt(&valid);
  if(!valid) { return false; }
  d_lineCount = integer;

  if(d_pointCount > 0) {
    d_points.reserve(d_pointCount*3);
    d_lineIds1.reserve(d_lineCount);
    d_lineIds2.reserve(d_lineCount);
  }

  int start = 3;
  for(int i=0; i<d_pointCount; i++) {
    value = data.section(':', i+start, i+start);
    integer = value.toInt(&valid);
    if(!valid) { return false; }
    pointIdToIndex[integer] = i;
    d_pointIndexToId[i] = integer;
  }

  start += d_pointCount;
  for(int i=0; i<(d_pointCount)*3; i++) {
    value = data.section(':', i+start, i+start);
    f = value.toDouble(&valid);
    if(!valid) { return false; }
    d_points.push_back(f);
  }

  if((unsigned int)d_pointCount != d_points.size()/3 ||
     ((unsigned int)d_pointCount <= 0 && d_lineCount > 0)) {
    cout << "1: " << d_pointCount << " " << d_points.size() << " ";
    cout << d_lineCount << endl;
    return false;
  }

  map<int, int>::iterator pid;
  start += ((d_pointCount) * 3);
  int index = 0;
  for(int i=0; i<d_lineCount; i++) {
    value = data.section(':', index+start, index+start);
    integer = value.toInt(&valid);
    if(!valid) { return false; }
    pid = pointIdToIndex.find(integer);
    if(pid == pointIdToIndex.end()) { return false; }
    d_lineIds1.push_back(pid->second);
    index++;

    value = data.section(':', index+start, index+start);
    integer = value.toInt(&valid);
    if(!valid) { return false; }
    pid = pointIdToIndex.find(integer);
    if(pid == pointIdToIndex.end()) { return false; }
    d_lineIds2.push_back(pid->second);
    index++;
  }

  if(d_lineIds1.size() != (unsigned int)d_lineCount ||
     d_lineIds2.size() != (unsigned int)d_lineCount) {
    return false;
  }

  d_valid = true;

  return true;
}

void Primative::Clear(void) {
  d_pointIndexToId.clear();
  d_points.clear();
  d_lineIds1.clear();
  d_lineIds2.clear();

  d_valid = false;

  return;
}
