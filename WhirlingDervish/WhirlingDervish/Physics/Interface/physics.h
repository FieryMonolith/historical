#ifndef __PHYSICS_H__
#define __PHYSICS_H__

class Vector3d;

// Physics calculations all use the following units
//   mass     -> kilograms
//   distance -> meters
//   time     -> seconds
//   force    -> Newtons

class Physics {
public:
  static void CalculateFrictionalForce(double staticMu, double kineticMu,
				       double mass, const Vector3d &force,
				       const Vector3d &velocity,
				       Vector3d &frictionalForce);
  static void CalculateAirResistance(double drag, 
				     const Vector3d &velocity,
				     Vector3d &resistanceForce);
  static void CalculateGravitationalForce(double mass,
					  const Vector3d &gravity,
					  Vector3d &gravitationalForce);

  static void Verlet(Vector3d &curPos, Vector3d &prevPos,
		     const Vector3d &force, double mass, double timeStep);
  static void SpringForce(Vector3d &force0, Vector3d &force1,
			  const Vector3d &p0, const Vector3d &prevP0,
			  const Vector3d &p1, const Vector3d &prevP1,
			  double ks, double kd, double innate_length,
			  double timeStep);

private:
  Physics(void);
  ~Physics(void);
};

#endif
