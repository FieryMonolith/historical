#include "physical_point_info.h"
#include "physical_point.h"
#include "spring_info.h"
#include "spring.h"
#include "jvector.h"

#include "shape_engine.h"

using namespace std;

ShapeEngine::ShapeEngine(void) : d_pointId(1), d_springId(1) {
  return;
}

ShapeEngine::~ShapeEngine(void) {
  vector<PhysicalPointInfo *>::iterator ip;
  for(ip = d_availablePoints.begin(); ip != d_availablePoints.end(); ++ip) {
    if(*ip == NULL) { continue; }

    delete *ip;
    *ip = NULL;
  }
  d_availablePoints.clear();

  vector<SpringInfo *>::iterator is;
  for(is = d_availableSprings.begin(); is != d_availableSprings.end(); ++is) {
    if(*is == NULL) { continue; }

    delete *is;
    *is = NULL;
  }
  d_availableSprings.clear();

  map<int64, SpringInfo *>::iterator ispring;
  for(ispring = d_springs.begin(); ispring != d_springs.end(); ++ispring) {
    if(ispring->second != NULL) {
      delete ispring->second;
      ispring->second = NULL;
    }
  }
  d_springs.clear();

  map<int64, PhysicalPointInfo *>::iterator point;
  for(point = d_points.begin(); point != d_points.end(); ++point) {
    if(point->second != NULL) {
      delete point->second;
      point->second = NULL;
    }
  }
  d_points.clear();

  return;
}

bool ShapeEngine::Initialize(void) {
  return true;
}

unsigned int ShapeEngine::PointCount(void) const {
  return d_points.size();
}

PhysicalPointInfo *ShapeEngine::CreatePointInfo(int64 &id,
						const Vector3d &position,
						double mass) {
  PhysicalPointInfo *ppi = NULL;

  if(d_availablePoints.empty()) {
    ppi = new PhysicalPointInfo;
  } else {
    ppi = d_availablePoints.back();
    d_availablePoints.pop_back();
  }

  if(ppi != NULL) {
    PhysicalPoint &p = ppi->GetPhysicalPoint();
    p.Position() = position;
    p.PreviousPosition() = position;
    p.Force() = Vector3d(0.0, 0.0, 0.0);
    p.Mass() = mass;

    d_pointId++;

    id = d_pointId - 1;
    ppi->Id() = id;
    ppi->OriginalPosition() = position;
    ppi->Pause() = false;
    ppi->Still() = false;

    d_points.insert(make_pair(id, ppi));
  }

  return ppi;
}

bool ShapeEngine::HasPoint(int64 id) const {
  map<int64, PhysicalPointInfo *>::const_iterator i = d_points.find(id);
  if(i == d_points.end() || i->second == NULL) {
    return false;
  }

  return true;
}

PhysicalPointInfo *ShapeEngine::GetPointInfo(int64 id) const {
  map<int64, PhysicalPointInfo *>::const_iterator i = d_points.find(id);
  if(i == d_points.end()) {
    return NULL;
  }

  return i->second;
}

void ShapeEngine::RemovePoint(int64 id) {
  map<int64, PhysicalPointInfo *>::iterator i = d_points.find(id);
  if(i == d_points.end()) {
    return;
  } else if(i->second == NULL) {
    d_points.erase(id);
    return;
  }

  d_availablePoints.push_back(i->second);
  i->second = NULL;
  d_points.erase(id);

  return;
}

unsigned int ShapeEngine::SpringCount(void) const {
  return d_springs.size();
}

SpringInfo *ShapeEngine::CreateSpringInfo(int64 &id,
					  double innateLength,
					  double ks,
					  double kd,
					  double fluctuation_delta,
					  double fluctuation_start,
					  double fluctuation_rate) {
  SpringInfo *si = NULL;

  if(d_availableSprings.empty()) {
    si = new SpringInfo;
  } else {
    si = d_availableSprings.back();
    d_availableSprings.pop_back();
  }

  if(si != NULL) {
    Spring &s = si->GetSpring();
    s.InnateLength() = innateLength;
    s.OriginalInnateLength() = innateLength;
    s.ks() = ks;
    s.kd() = kd;
    s.FluctuationDelta() = fluctuation_delta;
    s.FluctuationStart() = fluctuation_start;
    s.FluctuationRate() = fluctuation_rate;
    s.Increment() = 0.0;

    d_springId++;

    id = d_springId - 1;
    si->Id() = id;
    si->InnateLength() = innateLength;
    si->OriginalInnateLength() = innateLength;
    si->ks() = ks;
    si->kd() = kd;
    si->FluctuationDelta() = fluctuation_delta;
    si->FluctuationStart() = fluctuation_start;
    si->FluctuationRate()  = fluctuation_rate;
    si->Increment() = 0.0;

    d_springs.insert(make_pair(id, si));
  }

  return si;
}

bool ShapeEngine::HasSpring(int64 id) const {
  map<int64, SpringInfo *>::const_iterator i = d_springs.find(id);
  if(i == d_springs.end() || i->second == NULL) {
    return false;
  }

  return true;
}

SpringInfo *ShapeEngine::GetSpringInfo(int64 id) const {
  map<int64, SpringInfo *>::const_iterator i = d_springs.find(id);
  if(i == d_springs.end()) {
    return NULL;
  }

  return i->second;
}

void ShapeEngine::RemoveSpring(int64 id) {
  map<int64, SpringInfo *>::iterator i = d_springs.find(id);
  if(i == d_springs.end()) {
    return;
  } else if(i->second == NULL) {
    d_springs.erase(id);
    return;
  }

  d_availableSprings.push_back(i->second);
  i->second = NULL;
  d_springs.erase(id);

  return;
}
