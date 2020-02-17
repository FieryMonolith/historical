#include "physical_point.h"

PhysicalPoint::PhysicalPoint(const Vector3d &position, double mass)
  : d_curPos(position),
    d_prevPos(position),
    d_force(Vector3d(0.0, 0.0, 0.0)),
    d_mass(mass) {
  return;
}

PhysicalPoint::PhysicalPoint(const PhysicalPoint &p) {
  *this = p;

  return;
}

PhysicalPoint::~PhysicalPoint(void) {
  return;
}

PhysicalPoint &PhysicalPoint::operator=(const PhysicalPoint &p) {
  d_curPos = p.d_curPos;
  d_prevPos = p.d_prevPos;
  d_force = p.d_force;
  d_mass = p.d_mass;

  return *this;
}
