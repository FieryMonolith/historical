#include "jvector.h"

#include "physics.h"

#include <iostream>
using namespace std;

void Physics::CalculateFrictionalForce(double staticMu, double kineticMu,
				       double mass, const Vector3d &force,
				       const Vector3d &velocity,
				       Vector3d &frictionalForce) {
  double weight = 9.8 * mass;

  if(LengthSquared(velocity) == 0 && LengthSquared(force) == 0) {
    // Force is zero if there is no force or velocity, special case
    frictionalForce[0] = 0;
    frictionalForce[1] = 0;
    frictionalForce[2] = 0;
  } else if(LengthSquared(velocity) == 0 &&
	    LengthSquared(force) < (staticMu*weight) * (staticMu*weight)) {
    // If velocity is zero, the force has to overcome static friction.
    // Since it is less than static friction, the x,z force must be equal to
    // and opposite the force already along the x,z
    Vector3d f(force);
    double forceMagnitude = Normalize(f);
    frictionalForce[0] = -f[0];
    frictionalForce[1] = 0;
    frictionalForce[2] = -f[2];

    frictionalForce *= forceMagnitude;
  } else {
    // This assumes that kinetic friction is always less than static
    // friction force
    Vector3d v(velocity);
    Normalize(v);
    frictionalForce[0] = -v[0];
    frictionalForce[1] = 0;
    frictionalForce[2] = -v[2];

    double newMag = kineticMu * weight;
    if(newMag * newMag > LengthSquared(force)) {
      newMag = Length(force);
    }

    frictionalForce *= newMag;
  }

  return;
}

void Physics::CalculateAirResistance(double drag,
				     const Vector3d &velocity,
				     Vector3d &resistanceForce) {
  resistanceForce = velocity;
  double magnitude = Normalize(resistanceForce);
  double resistMag = -1 * magnitude * magnitude * drag;
  resistanceForce *= resistMag;
  
  return;
}

void Physics::CalculateGravitationalForce(double mass,
					  const Vector3d &gravity,
					  Vector3d &gravitationalForce) {
  gravitationalForce = gravity;
  gravitationalForce *= mass;

  return;
}

void Physics::Verlet(Vector3d &curPos, Vector3d &prevPos,
		     const Vector3d &force, double mass, double timeStep) {
  const Vector3d t(curPos);
  //curPos = (2.0 * curPos) - prevPos +
  //         (force * (timeStep * timeStep * 0.5) / mass);
  curPos += curPos - prevPos + (force * ((timeStep * timeStep * 0.5) / mass));
  prevPos = t;

  return;
}

void Physics::SpringForce(Vector3d &force0, Vector3d &force1,
			  const Vector3d &p0, const Vector3d &prevP0,
			  const Vector3d &p1, const Vector3d &prevP1,
			  double ks, double, double innateLength,
			  double timeStep) {
  // Determine direction of force
  Vector3d force(p1 - p0);
  double lineLength = Normalize(force);

  if(lineLength == 0) {
    double invTime = 1.0 / timeStep;
    Vector3d velocity(((p1 - prevP1) * invTime) - ((p0 - prevP0) * invTime));
    double velMag = Normalize(velocity);
    if(velMag == 0) {
      return;
    }

    force = velocity;
  }

  // Determine amount of force
  force *= ks * (lineLength - innateLength);

  force0 = force;
  force1 = -force;

  return;
}
