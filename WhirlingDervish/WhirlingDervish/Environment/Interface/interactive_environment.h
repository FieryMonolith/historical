#ifndef __INTERACTIVE_ENVIRONMENT_H__
#define __INTERACTIVE_ENVIRONMENT_H__

#include <string>
#include <ostream>

#include "defs.h"
#include "interactive_environment_interface.h"
#include "environment.h"
#include "interactive_environment_data.h"
#include "creature_info.h"

class Vector3d;
class Parser;
class iPopulationStorage;
class FileHandler;

class InteractiveEnvironment : public iInteractiveEnvironment,
			       protected Environment,
			       protected InteractiveEnvironmentData {
public:
  InteractiveEnvironment(int id);
  ~InteractiveEnvironment(void);

  // Execute these in this order
  bool Initialize(void);
  bool LoadConfiguration(FileHandler &,
			 const std::string &runSettingsFilename,
			 const std::string &physicsSettingsFilename,
			 const std::string &ecSettingsFilename,
			 const std::string &instructionSetFilename);
  bool InitializeExperiment(FileHandler &);
  void Shutdown(FileHandler &);

  bool Process(double timestep);
  bool ExperimentProcess(double timestep, FileHandler &);
  int CurrentGeneration(void) const;
  int MaxGeneration(void) const;

  RunSettings &GetRunSettings(void) {
    return EnvironmentData::GetRunSettings();
  }
  PhysicsSettings &GetPhysicsSettings(void) {
    return EnvironmentData::GetPhysicsSettings();
  }
  ECSettings &GetECSettings(void) { return EnvironmentData::GetECSettings(); }

  // Pause environment
  void Pause(void);
  void Unpause(void);
  bool Paused(void);

  // Select a creature
  void SelectCreature(int64 id);
  void DeselectCreature(void);

  // Delete Creature
  void DeleteCreature(int64 id);
  void DeleteSelected(void);

  // Reset creature
  void ResetCreature(int64 id);
  void ResetSelected(void);

  // Move by delta
  void MovePoint(int64 id, const Vector3d &shift);
  void MoveLine(int64 point1Id, int64 point2Id, const Vector3d &shift);
  void MoveSelected(const Vector3d &shift);
  void MoveCreature(int64 id, const Vector3d &shift);

  // Freeze
  void FreezePoint(int64 id);
  void FreezeLine(int64 point1Id, int64 point2Id, int64 springId);
  void FreezeSelected(void);
  void FreezeCreature(int64 id);

  // Unfreeze
  void UnfreezePoint(int64 id);
  void UnfreezeLine(int64 point1Id, int64 point2Id, int64 springId);
  void UnfreezeSelected(void);
  void UnfreezeCreature(int64 id);

  // Alter/Genome Information for a selected  creature
  bool GenomeSelected(std::ostream &) const;
  bool GenomeCreature(int64 id, std::ostream &) const;
  bool AlterSelected(const std::string &);
  bool AlterSelected(const std::string &, double fitness);
  bool AlterCreature(int64 id, const std::string &);
  bool AlterCreature(int64 id, const std::string &,
		     double fitness);
  bool InsertCreature(int64 &id, const std::string &);
  bool FillSelectedInfo(CreatureInfo &) const;
  bool FillCreatureInfo(int64 id, CreatureInfo &) const;

  Parser &GetParser(void);
  iPopulationStorage &GetPS(void);

  // Information
  double  Timestep(void) const { return d_physicsSettings.Timestep(); }
  double &Timestep(void)       { return d_physicsSettings.Timestep(); }
  double CurrentTime(void) const { return EnvironmentData::CurrentTime(); }
  double SimulationTime(void) const { return d_physicsSettings.Simtime(); }
  unsigned int GenerationQuantity(void) const {
    return d_runSettings.MaxGenerations();
  }
  unsigned int Generation(void) const { return EnvironmentData::Generation(); }
  unsigned int Language(void) const { return d_dummy; }

  double AvgFitness(void) const { return EnvironmentData::AvgFitness(); }
  double MaxFitness(void) const { return EnvironmentData::MaxFitness(); }
  double AvgPoints(void)  const { return EnvironmentData::AvgPoints(); }
  double AvgSprings(void) const { return EnvironmentData::AvgSprings(); }
  double AvgHeight(void)  const { return EnvironmentData::AvgHeight(); }
  double MaxHeight(void)  const { return EnvironmentData::MaxHeight(); }

  double  GravityX(void) const { return d_physicsSettings.GravityX(); }
  double &GravityX(void)       { return d_physicsSettings.GravityX(); }
  double  GravityY(void) const { return d_physicsSettings.GravityX(); }
  double &GravityY(void)       { return d_physicsSettings.GravityX(); }
  double  GravityZ(void) const { return d_physicsSettings.GravityX(); }
  double &GravityZ(void)       { return d_physicsSettings.GravityX(); }
  double  Drag(void) const     { return d_physicsSettings.Drag(); }
  double &Drag(void)           { return d_physicsSettings.Drag(); }

private:
  int d_id;
  int d_dummy;

  CreatureInfo *FindCreatureInfo(int64 id) const;
};

#endif
