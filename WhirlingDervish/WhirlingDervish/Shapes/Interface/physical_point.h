#ifndef __PHYSICAL_POINT_H__
#define __PHYSICAL_POINT_H__

#include "jvector.h"

class PhysicalPoint {
public:
  PhysicalPoint(const Vector3d &position = Vector3d(0.0, 0.0, 0.0),
		double mass = 0.0);
  PhysicalPoint(const PhysicalPoint &);
  ~PhysicalPoint(void);

  PhysicalPoint &operator=(const PhysicalPoint &);

  double  Mass(void)      const { return d_mass;      }
  double &Mass(void)            { return d_mass;      }

  void ChangeMass(double deltaMass) { d_mass += deltaMass; }

  const Vector3d &Position(void)         const { return d_curPos;  }
        Vector3d &Position(void)               { return d_curPos;  }
  const Vector3d &PreviousPosition(void) const { return d_prevPos; }
        Vector3d &PreviousPosition(void)       { return d_prevPos; }

  Vector3d Velocity(double timestep) {
    if(timestep == 0) { return 0; }
    return (d_curPos - d_prevPos) / timestep;
  }

  void AddForce(const Vector3d &force) { d_force += force; return; }
  const Vector3d &Force(void) const { return d_force; }
        Vector3d &Force(void)       { return d_force; }

private:
  Vector3d d_curPos;
  Vector3d d_prevPos;
  Vector3d d_force;

  double d_mass;
};

// These are for management, so use at your own risk.  They assume that
// the other side of the binding will also be released in the same fashion
void SetMass(PhysicalPoint &p, double newMass);
bool Initialize(PhysicalPoint &p, const Vector3d &position, double mass);
void Reset(PhysicalPoint &p, const Vector3d &originalPosition,
	   const Vector3d &originalPreviousPosition);
bool FreeBindings(PhysicalPoint &p);

#endif
