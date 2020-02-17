#ifndef __CREATURE_INFO_H__
#define __CREATURE_INFO_H__

#include "defs.h"
#include "creature.h"
#include "jvector.h"
#include <vector>

class EnvironmentData;

class CreatureInfo {
public:
  CreatureInfo(void);
  ~CreatureInfo(void);

  bool Clean(EnvironmentData &);

  const Creature &GetCreature(void) const { return d_creature; }
        Creature &GetCreature(void)       { return d_creature; }

  int64  Id(void) const { return d_id; }
  int64 &Id(void)       { return d_id; }

  unsigned  Clade(void) const { return d_clade; }
  unsigned &Clade(void)       { return d_clade; }

  unsigned  parentClade(void) const { return d_clade; }
  unsigned &parentClade(void)       { return d_parentClade; }

  int64  ParentId(void) const { return d_parentId; }
  int64 &ParentId(void)       { return d_parentId; }

  double  Fitness(void) const { return d_fitness; }
  double &Fitness(void)       { return d_fitness; }

  double  parentFitness(void) const { return d_parentFitness; }
  double &parentFitness(void)       { return d_parentFitness; }

  std::vector<unsigned>  vFitness(void) const { return v_fitness; }
  std::vector<unsigned> &vFitness(void)       { return v_fitness; }

  std::vector<double>  Fitnesses(void) const {return fitVec;}
  std::vector<double> &Fitnesses(void)       {return fitVec;}

  bool  Pause(void) const { return d_pause; }
  bool &Pause(void)       { return d_pause; }

  bool  Still(void) const { return d_still; }
  bool &Still(void)       { return d_still; }

  void BodyReset(void);

  const Vector3d &CenterOfMass(void) const { return d_centerOfMass; }
        Vector3d &CenterOfMass(void)       { return d_centerOfMass; }

  double  DistanceMoved(void) const { return d_distanceMoved; }
  double &DistanceMoved(void)       { return d_distanceMoved; }

  double  DistanceY(void) const { return d_distanceY; }
  double &DistanceY(void)       { return d_distanceY; }

  const std::map<int64,double> &Interdistance(void) const {
    return d_interdistance;
  }
  std::map<int64,double> &Interdistance(void) { return d_interdistance; }

  double  InterdistanceQuantity(void) const { return d_interdistanceQuantity; }
  double &InterdistanceQuantity(void)       { return d_interdistanceQuantity; }

  const Vector3d &OriginalCenterOfMass(void) const { return d_oCenterOfMass; }
        Vector3d &OriginalCenterOfMass(void)       { return d_oCenterOfMass; }

  const double  OriginalHeight(void) const { return d_originalHeight; }
        double &OriginalHeight(void)       { return d_originalHeight; }

  const double  OriginalMinY(void) const { return d_originalMinY; }
        double &OriginalMinY(void)       { return d_originalMinY; }

  const Vector3d &COMPosition30(void) const { return d_comPosition30; }
        Vector3d &COMPosition30(void)       { return d_comPosition30; }

  const Vector3d &COMPosition75(void) const { return d_comPosition75; }
        Vector3d &COMPosition75(void)       { return d_comPosition75; }

  const Vector3d &COMPosition60(void) const { return d_comPosition60; }
        Vector3d &COMPosition60(void)       { return d_comPosition60; }

  // Determines if a < b
  static bool Less(const CreatureInfo *a, const CreatureInfo *b) {
    if(a == NULL || b == NULL) { return false; }
    if(a->Fitness() < b->Fitness()) { return true; }
    return false;
  }

  // Determines if a > b
  static bool Greater(const CreatureInfo *a, const CreatureInfo *b) {
    if(a == NULL || b == NULL) { return false; }
    if(a->Fitness() > b->Fitness()) { return true; }
    return false;
  }

private:
  Creature d_creature;
  int64 d_id;
  int64 d_parentId;
  double d_fitness;
  double d_parentFitness;
  bool d_pause;
  bool d_still;
  Vector3d d_centerOfMass;
  double d_originalHeight;
  double d_originalMinY;
  double d_distanceMoved;
  double d_distanceY;
  Vector3d d_oCenterOfMass;
  std::map<int64, double> d_interdistance;
  double d_interdistanceQuantity;

  unsigned d_clade;
  unsigned d_parentClade;
  std::vector<unsigned> v_fitness;
  std::vector<double>  fitVec;

  Vector3d d_comPosition30;
  Vector3d d_comPosition75;
  Vector3d d_comPosition60;
};

#endif
