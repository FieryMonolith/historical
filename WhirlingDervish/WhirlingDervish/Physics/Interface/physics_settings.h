#ifndef __PHYSICS_SETTINGS_H__
#define __PHYSICS_SETTINGS_H__

#include <string>

#include "jvector.h"
#include "setting.h"

class PhysicsSettings : public Setting {
public:
  PhysicsSettings(void);
  ~PhysicsSettings(void);

  bool Initialize(void);

  double  Timestep(void) const { return d_timestep; }
  double &Timestep(void)       { return d_timestep; }

  double  Simtime(void) const { return d_simtime; }
  double &Simtime(void)       { return d_simtime; }

  double  GravityX(void) const { return d_gravityX; }
  double &GravityX(void)       { return d_gravityX; }

  double  GravityY(void) const { return d_gravityY; }
  double &GravityY(void)       { return d_gravityY; }

  double  GravityZ(void) const { return d_gravityZ; }
  double &GravityZ(void)       { return d_gravityZ; }

  Vector3d Gravity(void) const {
    return Vector3d(d_gravityX, d_gravityY, d_gravityZ);
  }

  double  Mass(void) const { return d_mass; }
  double &Mass(void)       { return d_mass; }

  double  MinMass(void) const { return d_minMass; }
  double &MinMass(void)       { return d_minMass; }

  double  MaxMass(void) const { return d_maxMass; }
  double &MaxMass(void)       { return d_maxMass; }

  double  StaticMu(void) const { return d_staticMu; }
  double &StaticMu(void)       { return d_staticMu; }

  double  KineticMu(void) const { return d_kineticMu; }
  double &KineticMu(void)       { return d_kineticMu; }

  double  Drag(void) const { return d_drag; }
  double &Drag(void)       { return d_drag; }

  double  InnateLength(void) const { return d_innateLength; }
  double &InnateLength(void)       { return d_innateLength; }

  double  KS(void) const { return d_KS; }
  double &KS(void)       { return d_KS; }

  double  MinKS(void) const { return d_minKS; }
  double &MinKS(void)       { return d_minKS; }

  double  MaxKS(void) const { return d_maxKS; }
  double &MaxKS(void)       { return d_maxKS; }

  double  MaxPercentFluctuation(void) const { return d_maxPercentFluctuation; }
  double &MaxPercentFluctuation(void)       { return d_maxPercentFluctuation; }

  double  FluctuationDelta(void) const { return d_fluctuationDelta; }
  double &FluctuationDelta(void)       { return d_fluctuationDelta; }

  double  MinFluctuationDelta(void) const { return d_minFluctuationDelta; }
  double &MinFluctuationDelta(void)       { return d_minFluctuationDelta; }

  double  MaxFluctuationDelta(void) const { return d_maxFluctuationDelta; }
  double &MaxFluctuationDelta(void)       { return d_maxFluctuationDelta; }

  double  FluctuationStart(void) const { return d_fluctuationStart; }
  double &FluctuationStart(void)       { return d_fluctuationStart; }

  double  MinFluctuationStart(void) const { return d_minFluctuationStart; }
  double &MinFluctuationStart(void)       { return d_minFluctuationStart; }

  double  MaxFluctuationStart(void) const { return d_maxFluctuationStart; }
  double &MaxFluctuationStart(void)       { return d_maxFluctuationStart; }

  double  FluctuationRate(void) const { return d_fluctuationRate; }
  double &FluctuationRate(void)       { return d_fluctuationRate; }

  double  MinFluctuationRate(void) const { return d_minFluctuationRate; }
  double &MinFluctuationRate(void)       { return d_minFluctuationRate; }

  double  MaxFluctuationRate(void) const { return d_maxFluctuationRate; }
  double &MaxFluctuationRate(void)       { return d_maxFluctuationRate; }

private:
  double d_timestep;
  double d_simtime;
  double d_gravityX;
  double d_gravityY;
  double d_gravityZ;
  double d_mass;
  double d_minMass;
  double d_maxMass;
  double d_staticMu;
  double d_kineticMu;
  double d_drag;
  double d_innateLength;
  double d_KS;
  double d_minKS;
  double d_maxKS;
  double d_maxPercentFluctuation;
  double d_fluctuationDelta;
  double d_minFluctuationDelta;
  double d_maxFluctuationDelta;
  double d_fluctuationStart;
  double d_minFluctuationStart;
  double d_maxFluctuationStart;
  double d_fluctuationRate;
  double d_minFluctuationRate;
  double d_maxFluctuationRate;

  static const std::string n_timestep;
  static const std::string n_simtime;
  static const std::string n_gravityX;
  static const std::string n_gravityY;
  static const std::string n_gravityZ;
  static const std::string n_mass;
  static const std::string n_minMass;
  static const std::string n_maxMass;
  static const std::string n_staticMu;
  static const std::string n_kineticMu;
  static const std::string n_drag;
  static const std::string n_innateLength;
  static const std::string n_KS;
  static const std::string n_minKS;
  static const std::string n_maxKS;
  static const std::string n_maxPercentFluctuation;
  static const std::string n_fluctuationDelta;
  static const std::string n_minFluctuationDelta;
  static const std::string n_maxFluctuationDelta;
  static const std::string n_fluctuationStart;
  static const std::string n_minFluctuationStart;
  static const std::string n_maxFluctuationStart;
  static const std::string n_fluctuationRate;
  static const std::string n_minFluctuationRate;
  static const std::string n_maxFluctuationRate;
};

#endif
