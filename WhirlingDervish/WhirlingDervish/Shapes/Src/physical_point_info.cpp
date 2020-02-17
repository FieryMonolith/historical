#include "physical_point_info.h"

PhysicalPointInfo::PhysicalPointInfo(void) : d_id(0), d_pause(false),
					     d_still(false) {
  return;
}

PhysicalPointInfo::PhysicalPointInfo(const PhysicalPointInfo &ppi) {
  *this = ppi;

  return;
}

PhysicalPointInfo::~PhysicalPointInfo(void) {
  return;
}

PhysicalPointInfo &PhysicalPointInfo::operator=(const PhysicalPointInfo &ppi) {
  d_point = ppi.d_point;
  d_id = ppi.d_id;
  d_originalPosition = ppi.d_originalPosition;
  d_pause = ppi.d_pause;
  d_still = ppi.d_still;

  return *this;
}

void PhysicalPointInfo::Reset(void) {
  d_point.Force() = Vector3d(0.0, 0.0, 0.0);
  d_point.Position() = d_originalPosition;
  d_point.PreviousPosition() = d_originalPosition;

  return;
}
