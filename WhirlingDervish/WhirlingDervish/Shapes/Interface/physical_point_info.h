#ifndef __PHYSICAL_POINT_INFO_H__
#define __PHYSICAL_POINT_INFO_H__

#include "defs.h"
#include "jvector.h"
#include "physical_point.h"

class PhysicalPointInfo {
public:
  PhysicalPointInfo(void);
  PhysicalPointInfo(const PhysicalPointInfo &);
  ~PhysicalPointInfo(void);

  PhysicalPointInfo &operator=(const PhysicalPointInfo &);

  void Reset(void);

  const PhysicalPoint &GetPhysicalPoint(void) const { return d_point; }
        PhysicalPoint &GetPhysicalPoint(void)       { return d_point; }

  int64  Id(void) const { return d_id; }
  int64 &Id(void)       { return d_id; }

  const Vector3d &OriginalPosition(void) const { return d_originalPosition; }
        Vector3d &OriginalPosition(void)       { return d_originalPosition; }

  bool  Pause(void) const { return d_pause; }
  bool &Pause(void)       { return d_pause; }

  bool  Still(void) const { return d_still; }
  bool &Still(void)       { return d_still; }

private:
  PhysicalPoint d_point;
  int64 d_id;

  Vector3d d_originalPosition;

  bool d_pause;
  bool d_still;
};

#endif
