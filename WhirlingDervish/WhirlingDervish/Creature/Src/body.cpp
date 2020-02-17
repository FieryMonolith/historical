#include "jvector.h"
#include "physical_point.h"
#include "physical_point_info.h"
#include "spring.h"
#include "spring_info.h"
#include "environment_data.h"

#include "body.h"

#include "shape_engine.h"

using namespace std;

Body::ConnectionData::ConnectionData(void)
  : d_spring(NULL), d_point1(NULL), d_point2(NULL) {
  return;
}

Body::ConnectionData::ConnectionData(SpringInfo *s,
				     PhysicalPointInfo *p1,
				     PhysicalPointInfo *p2)
  : d_spring(s), d_point1(p1), d_point2(p2) {
  return;
}

Body::ConnectionData::~ConnectionData(void) {
  d_spring = NULL;
  d_point1 = NULL;
  d_point2 = NULL;

  return;
}

Body::Body(void) {
  return;
}

Body::~Body(void) {
  return;
}

int Body::PointCount(void) const {
  return d_points.size();
}

bool Body::HasPoint(int64 id) const {
  map<int64, PhysicalPointInfo *>::const_iterator i = d_points.find(id);
  if(i == d_points.end()) {
    return false;
  }

  return true;
}

PhysicalPointInfo *Body::AddPoint(EnvironmentData &ed,
				  int64 &id, const Vector3d &position,
				  double mass) {
  if(mass < 0) {
    return NULL;
  }

  PhysicalPointInfo *point = ed.GetShapeEngine().CreatePointInfo(id,
								 position,
								 mass);
  if(point == NULL) {
    return NULL;
  }

  d_points.insert(make_pair(id, point));

  return point;
}

void Body::RemovePoint(EnvironmentData &ed, int64 pointId) {
  map<int64, PhysicalPointInfo *>::iterator test = d_points.find(pointId);
  if(test == d_points.end()) { return; }

  d_points.erase(pointId);

  vector<int64> temp;
  map<int64, ConnectionData>::iterator i;
  for(i=d_connections.begin(); i!= d_connections.end(); ++i) {
    if((i->second.d_point1 != NULL && pointId == i->second.d_point1->Id()) ||
       (i->second.d_point2 != NULL && pointId == i->second.d_point2->Id())) {
      temp.push_back(i->first);
    }
  }

  ShapeEngine &se = ed.GetShapeEngine();

  vector<int64>::iterator ii;
  for(ii = temp.begin(); ii != temp.end(); ++ii) {
    se.RemoveSpring(*ii);
    d_springs.erase(*ii);
    d_connections.erase(*ii);
  }

  se.RemovePoint(pointId);

  return;
}

PhysicalPointInfo *Body::GetPoint(int64 id) const {
  map<int64, PhysicalPointInfo *>::const_iterator i = d_points.find(id);
  if(i == d_points.end()) {
    return NULL;
  }

  return i->second;
}

const map<int64, PhysicalPointInfo *> &Body::GetPoints(void) const {
  return d_points;
}

int Body::SpringCount(void) const {
  return d_springs.size();
}

bool Body::HasSpring(int64 id) const {
  map<int64, SpringInfo *>::const_iterator i = d_springs.find(id);
  if(i == d_springs.end()) {
    return false;
  }

  return true;
}

SpringInfo *Body::AddSpring(EnvironmentData &ed,
			    int64 &id,
			    PhysicalPointInfo *p1,
			    PhysicalPointInfo *p2,
			    double innateLength,
			    double ks,
			    double kd,
			    double fluctuationDelta,
			    double fluctuationStart,
			    double fluctuationRate) {
  if(p1 == NULL || p2 == NULL) {
    return NULL;
  }

  SpringInfo *spring = ed.GetShapeEngine().CreateSpringInfo(id,
							    innateLength,
							    ks,
							    kd,
							    fluctuationDelta,
							    fluctuationStart,
							    fluctuationRate);
  if(spring == NULL) {
    return NULL;
  }

  d_springs.insert(make_pair(id, spring));
  d_connections.insert(make_pair(id,
				 ConnectionData(spring, p1, p2)));

  return spring;
}

void Body::RemoveSpring(EnvironmentData &ed, int64 springId) {
  map<int64, SpringInfo *>::iterator test = d_springs.find(springId);
  if(test == d_springs.end()) { return; }

  d_springs.erase(springId);
  d_connections.erase(springId);
  ed.GetShapeEngine().RemoveSpring(springId);

  return;
}

SpringInfo *Body::GetSpring(int64 id) const {
  map<int64, SpringInfo *>::const_iterator i = d_springs.find(id);
  if(i == d_springs.end()) {
    return NULL;
  }

  return i->second;
}

const map<int64, SpringInfo *> &Body::GetSprings(void) const {
  return d_springs;
}

const map<int64, Body::ConnectionData> &Body::Connections(void) const {
  return d_connections;
}

void Body::Clear(EnvironmentData &ed) {
  ShapeEngine &se = ed.GetShapeEngine();

  map<int64, PhysicalPointInfo *>::iterator ip;
  for(ip = d_points.begin(); ip != d_points.end(); ++ip) {
    se.RemovePoint(ip->first);
    ip->second = NULL;
  }

  map<int64, SpringInfo *>::iterator im;
  for(im = d_springs.begin(); im != d_springs.end(); ++im) {
    se.RemoveSpring(im->first);
    im->second = NULL;
  }

  d_points.clear();
  d_springs.clear();
  d_connections.clear();

  return;
}
