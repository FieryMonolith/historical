#include "environment_data.h"

#include "creature_info.h"

#include "physical_point_info.h"
#include "spring_info.h"

using namespace std;

CreatureInfo::CreatureInfo(void) : d_id(0), d_parentId(0),
				   d_fitness(0.0),
				   d_pause(false), d_still(false),
				   d_centerOfMass(Vector3d(0.0, 0.0, 0.0)),
				   d_distanceMoved(0.0),
				   d_distanceY(0.0),
				   d_interdistanceQuantity(0.0) {
  return;
}

CreatureInfo::~CreatureInfo(void) {
  return;
}

bool CreatureInfo::Clean(EnvironmentData &ed) {
  d_fitness = 0.0;
  d_parentFitness = 0.0;
  d_pause = false;
  d_still = false;
  d_centerOfMass = Vector3d(0.0, 0.0, 0.0);
  d_originalHeight = 0.0;
  d_originalMinY = 0.0;
  d_distanceMoved = 0.0;
  d_distanceY = 0.0;
  d_oCenterOfMass = Vector3d(0.0, 0.0, 0.0);
  d_interdistance.clear();
  d_interdistanceQuantity = 0.0;

  d_clade = 0;
  d_parentClade = 0;
  v_fitness.clear();

  d_comPosition30 = Vector3d(0.0, 0.0, 0.0);
  d_comPosition75 = Vector3d(0.0, 0.0, 0.0);
  d_comPosition60 = Vector3d(0.0, 0.0, 0.0);

  return d_creature.Kill(ed);
}

void CreatureInfo::BodyReset(void) {
  d_parentFitness = 0.0;
  d_centerOfMass = Vector3d(0.0, 0.0, 0.0);
  d_originalHeight = 0.0;
  d_originalMinY = 0.0;
  d_distanceMoved = 0.0;
  d_distanceY = 0.0;
  d_oCenterOfMass = Vector3d(0.0, 0.0, 0.0);
  d_interdistance.clear();
  d_interdistanceQuantity = 0.0;
  d_comPosition30 = Vector3d(0.0, 0.0, 0.0);
  d_comPosition75 = Vector3d(0.0, 0.0, 0.0);
  d_comPosition60 = Vector3d(0.0, 0.0, 0.0);

  Body &b = GetCreature().GetBody();

  const map<int64, PhysicalPointInfo *> &points = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;
  for(ip=points.begin(); ip!=points.end(); ++ip) {
    if(ip->second == NULL) { continue; }

    ip->second->Reset();
  }

  const map<int64, SpringInfo *> &springs = b.GetSprings();
  map<int64, SpringInfo *>::const_iterator is;
  for(is=springs.begin(); is!=springs.end(); ++is) {
    if(is->second == NULL) { continue; }

    is->second->Reset();
  }

  return;
}
