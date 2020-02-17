#ifndef __BODY_H__
#define __BODY_H__

#include <map>
// Need utility for std::pair
#include <utility>

#include "defs.h"

class Vector3d;
class PhysicalPoint;
class PhysicalPointInfo;
class Spring;
class SpringInfo;
class EnvironmentData;

class Body {
public:
  class ConnectionData {
  public:
    ConnectionData(void);
    ConnectionData(SpringInfo *, PhysicalPointInfo *, PhysicalPointInfo *);
    ~ConnectionData(void);

    SpringInfo *d_spring;
    PhysicalPointInfo *d_point1;
    PhysicalPointInfo *d_point2;
  };

  Body(void);
  ~Body(void);

  void Clear(EnvironmentData &);

  // POINT METHODS
  int PointCount(void) const;
  bool HasPoint(int64 id) const;
  PhysicalPointInfo *AddPoint(EnvironmentData &ed,
			      int64 &id, const Vector3d &position, 
			      double mass);
  void RemovePoint(EnvironmentData &ed, int64 pointId);
  PhysicalPointInfo *GetPoint(int64 id) const;
  const std::map<int64, PhysicalPointInfo *> &GetPoints(void) const;

  // SPRING METHODS
  int SpringCount(void) const;
  bool HasSpring(int64 id) const;
  SpringInfo *AddSpring(EnvironmentData &ed, int64 &id,
			PhysicalPointInfo *p1, PhysicalPointInfo *p2,
			double innateLength, double ks, 
			double kd, double fluctuationDelta,
			double fluctuationStart, double fluctuationRate);
  void RemoveSpring(EnvironmentData &ed, int64 springId);
  SpringInfo *GetSpring(int64 id) const;
  const std::map<int64, SpringInfo *> &GetSprings(void) const;

  const std::map<int64, ConnectionData> &Connections(void) const;

private:
  // Does not own these, just quick references to the real ones
  std::map<int64, PhysicalPointInfo *> d_points;
  std::map<int64, SpringInfo *> d_springs;
  std::map<int64, ConnectionData> d_connections;
};

#endif
