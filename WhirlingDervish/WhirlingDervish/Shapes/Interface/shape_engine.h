#ifndef __SHAPE_ENGINE_H__
#define __SHAPE_ENGINE_H__

#include <map>
#include <vector>

#include "defs.h"

class Vector3d;
class PhysicalPoint;
class PhysicalPointInfo;
class Spring;
class SpringInfo;

class ShapeEngine {
public:
  ShapeEngine(void);
  ~ShapeEngine(void);

  // Trying to make a common interface for engines, but this engine starts gtg
  bool Initialize(void);

  unsigned int PointCount(void) const;
  PhysicalPointInfo *CreatePointInfo(int64 &id, const Vector3d &position,
				     double mass);
  bool HasPoint(int64 id) const;
  PhysicalPointInfo *GetPointInfo(int64 id) const;
  void RemovePoint(int64 id);
  const std::map<int64, PhysicalPointInfo *> &Points(void) const {
    return d_points;
  }

  unsigned int SpringCount(void) const;
  SpringInfo *CreateSpringInfo(int64 &id,
			       double innateLength,
			       double ks,
			       double kd,
			       double fluctuation_delta,
			       double fluctuation_start,
			       double fluctuation_rate);
  bool HasSpring(int64 id) const;
  SpringInfo *GetSpringInfo(int64 id) const;
  void RemoveSpring(int64 id);
  const std::map<int64, SpringInfo *> &Springs(void) const {return d_springs;}

private:
  std::map<int64, PhysicalPointInfo *> d_points;
  int64 d_pointId;

  std::map<int64, SpringInfo *> d_springs;
  int64 d_springId;

  std::vector<PhysicalPointInfo *> d_availablePoints;
  std::vector<SpringInfo *> d_availableSprings;
};

#endif

