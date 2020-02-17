#include "physics_settings.h"

using namespace std;

const string PhysicsSettings::n_timestep("TIME_STEP");
const string PhysicsSettings::n_simtime("SIM_TIME");
const string PhysicsSettings::n_gravityX("GRAVITY_X");
const string PhysicsSettings::n_gravityY("GRAVITY_Y");
const string PhysicsSettings::n_gravityZ("GRAVITY_Z");
const string PhysicsSettings::n_mass("MASS");
const string PhysicsSettings::n_minMass("MIN_MASS");
const string PhysicsSettings::n_maxMass("MAX_MASS");
const string PhysicsSettings::n_staticMu("STATIC_MU");
const string PhysicsSettings::n_kineticMu("KINETIC_MU");
const string PhysicsSettings::n_drag("DRAG");
const string PhysicsSettings::n_innateLength("INNATE_LENGTH");
const string PhysicsSettings::n_KS("KS");
const string PhysicsSettings::n_minKS("MIN_KS");
const string PhysicsSettings::n_maxKS("MAX_KS");
const string PhysicsSettings::n_maxPercentFluctuation("MAX_PERCENT_FLUCTUATION");
const string PhysicsSettings::n_fluctuationDelta("FLUCTUATION_DELTA");
const string PhysicsSettings::n_minFluctuationDelta("MIN_FLUCTUATION_DELTA");
const string PhysicsSettings::n_maxFluctuationDelta("MAX_FLUCTUATION_DELTA");
const string PhysicsSettings::n_fluctuationStart("FLUCTUATION_START");
const string PhysicsSettings::n_minFluctuationStart("MIN_FLUCTUATION_START");
const string PhysicsSettings::n_maxFluctuationStart("MAX_FLUCTUATION_START");
const string PhysicsSettings::n_fluctuationRate("FLUCTUATION_RATE");
const string PhysicsSettings::n_minFluctuationRate("MIN_FLUCTUATION_RATE");
const string PhysicsSettings::n_maxFluctuationRate("MAX_FLUCTUATION_RATE");

PhysicsSettings::PhysicsSettings(void)
  : d_timestep(1.0),
    d_simtime(0.0),
    d_gravityX(0.0),
    d_gravityY(0.0),
    d_gravityZ(0.0),
    d_mass(0.000000000001),
    d_minMass(0.000000000001),
    d_maxMass(0.000000000001),
    d_staticMu(0.0),
    d_kineticMu(0.0),
    d_drag(0.0),
    d_innateLength(0.000000000001),
    d_KS(0.0),
    d_minKS(0.0),
    d_maxKS(0.0),
    d_maxPercentFluctuation(0.0),
    d_fluctuationDelta(0.0),
    d_minFluctuationDelta(0.0),
    d_maxFluctuationDelta(0.0),
    d_fluctuationStart(0.0),
    d_minFluctuationStart(0.0),
    d_maxFluctuationStart(0.0),
    d_fluctuationRate(0.0),
    d_minFluctuationRate(0.0),
    d_maxFluctuationRate(0.0)
{
  return;
}

PhysicsSettings::~PhysicsSettings(void) {
  return;
}

bool PhysicsSettings::Initialize(void) {
  Destroy();

  SettingParser::InputBase *ib;

  // PhysicsSettings::Timestep
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_timestep, this,
						  &PhysicsSettings::Timestep);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_timestep, ib));

  // PhysicsSettings::Simtime
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_simtime, this,
						  &PhysicsSettings::Simtime);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_simtime, ib));

  // PhysicsSettings::GravityX
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_gravityX, this,
						  &PhysicsSettings::GravityX);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_gravityX, ib));

  // PhysicsSettings::GravityY
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_gravityY, this,
						  &PhysicsSettings::GravityY);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_gravityY, ib));

  // PhysicsSettings::GravityZ
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_gravityZ, this,
						  &PhysicsSettings::GravityZ);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_gravityZ, ib));

  // PhysicsSettings::Mass
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_mass, this,
						  &PhysicsSettings::Mass);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_mass, ib));

  // PhysicsSettings::MinMass
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_minMass, this,
						  &PhysicsSettings::MinMass);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_minMass, ib));

  // PhysicsSettings::MaxMass
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_maxMass, this,
						  &PhysicsSettings::MaxMass);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxMass, ib));

  // PhysicsSettings::StaticMu
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_staticMu, this,
						  &PhysicsSettings::StaticMu);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_staticMu, ib));

  // PhysicsSettings::KineticMu
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_kineticMu, this,
						  &PhysicsSettings::KineticMu);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_kineticMu, ib));

  // PhysicsSettings::Drag
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_drag, this,
						  &PhysicsSettings::Drag);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_drag, ib));

  // PhysicsSettings::InnateLength
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_innateLength,
					       this,
					       &PhysicsSettings::InnateLength);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_innateLength, ib));

  // PhysicsSettings::KS
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_KS, this,
						  &PhysicsSettings::KS);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_KS, ib));

  // PhysicsSettings::MinKS
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_minKS, this,
						  &PhysicsSettings::MinKS);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_minKS, ib));

  // PhysicsSettings::MaxKS
  ib = new SettingParser::Input<double &, PhysicsSettings>(n_maxKS, this,
						  &PhysicsSettings::MaxKS);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxKS, ib));

  // PhysicsSettings::PercentFluctuation
  ib = new SettingParser::Input<double &, PhysicsSettings>(
				      n_maxPercentFluctuation, this,
				      &PhysicsSettings::MaxPercentFluctuation);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxPercentFluctuation, ib));

  // PhysicsSettings::FluctuationDelta
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					   n_fluctuationDelta, this,
					   &PhysicsSettings::FluctuationDelta);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_fluctuationDelta, ib));

  // PhysicsSettings::MinFluctuationDelta
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_minFluctuationDelta, this,
					&PhysicsSettings::MinFluctuationDelta);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_minFluctuationDelta, ib));

  // PhysicsSettings::MaxFluctuationDelta
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_maxFluctuationDelta, this,
					&PhysicsSettings::MaxFluctuationDelta);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxFluctuationDelta, ib));

  // PhysicsSettings::FluctuationStart
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_fluctuationStart, this,
					&PhysicsSettings::FluctuationStart);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_fluctuationStart, ib));

  // PhysicsSettings::MinFluctuationStart
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_minFluctuationStart, this,
					&PhysicsSettings::MinFluctuationStart);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_minFluctuationStart, ib));

  // PhysicsSettings::MaxFluctuationStart
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_maxFluctuationStart, this,
					&PhysicsSettings::MaxFluctuationStart);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxFluctuationStart, ib));

  // PhysicsSettings::FluctuationRate
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_fluctuationRate, this,
					&PhysicsSettings::FluctuationRate);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_fluctuationRate, ib));

  // PhysicsSettings::MinFluctuationRate
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_minFluctuationRate, this,
					&PhysicsSettings::MinFluctuationRate);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_minFluctuationRate, ib));

  // PhysicsSettings::MaxFluctuationRate
  ib = new SettingParser::Input<double &, PhysicsSettings>(
					n_maxFluctuationRate, this,
					&PhysicsSettings::MaxFluctuationRate);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxFluctuationRate, ib));

  return true;
}
