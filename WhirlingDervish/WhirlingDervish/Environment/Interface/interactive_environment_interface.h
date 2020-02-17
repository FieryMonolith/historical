#ifndef __INTERACTIVE_ENVIRONMENT_INTERFACE_H__
#define __INTERACTIVE_ENVIRONMENT_INTERFACE_H__


#include <string>
#include <ostream>

#include "defs.h"

class Vector3d;
class CreatureInfo;
class Parser;
class iPopulationStorage;
class RunSettings;
class ECSettings;
class PhysicsSettings;

class iInteractiveEnvironment {
public:
  iInteractiveEnvironment(void) { return; }
  virtual ~iInteractiveEnvironment(void) { return; }

  virtual RunSettings &GetRunSettings(void) = 0;
  virtual PhysicsSettings &GetPhysicsSettings(void) = 0;
  virtual ECSettings &GetECSettings(void) = 0;

  // Pause environment
  virtual void Pause(void) = 0;
  virtual void Unpause(void) = 0;
  virtual bool Paused(void) = 0;

  // Select a creature
  virtual void SelectCreature(int64 id) = 0;
  virtual void DeselectCreature(void) = 0;

  // Delete Creature
  virtual void DeleteCreature(int64 id) = 0;
  virtual void DeleteSelected(void) = 0;

  // Reset creature
  virtual void ResetCreature(int64 id) = 0;
  virtual void ResetSelected(void) = 0;

  // Move by delta
  virtual void MovePoint(int64 id, const Vector3d &shift) = 0;
  virtual void MoveLine(int64 point1Id, int64 point2Id,
			const Vector3d &shift) = 0;
  virtual void MoveSelected(const Vector3d &shift) = 0;
  virtual void MoveCreature(int64 id, const Vector3d &shift) = 0;

  // Freeze
  virtual void FreezePoint(int64 id) = 0;
  virtual void FreezeLine(int64 point1Id, int64 point2Id, int64 springId) = 0;
  virtual void FreezeSelected(void) = 0;
  virtual void FreezeCreature(int64 id) = 0;

  // Unfreeze
  virtual void UnfreezePoint(int64 id) = 0;
  virtual void UnfreezeLine(int64 point1Id, int64 point2Id,
			    int64 springId) = 0;
  virtual void UnfreezeSelected(void) = 0;
  virtual void UnfreezeCreature(int64 id) = 0;

  // Alter/Genome Information for a selected  creature
  virtual bool GenomeSelected(std::ostream &) const = 0;
  virtual bool GenomeCreature(int64 id, std::ostream &) const = 0;
  virtual bool AlterSelected(const std::string &) = 0;
  virtual bool AlterSelected(const std::string &, double fitness) = 0;
  virtual bool AlterCreature(int64 id, const std::string &, double fitness)=0;
  virtual bool AlterCreature(int64 id, const std::string &) = 0;
  virtual bool InsertCreature(int64 &id, const std::string &)=0;

  virtual bool FillSelectedInfo(CreatureInfo &) const = 0;
  virtual bool FillCreatureInfo(int64 id, CreatureInfo &) const = 0;

  virtual Parser &GetParser(void) = 0;
  virtual iPopulationStorage &GetPS(void) = 0;

  // Information
  virtual double  Timestep(void) const = 0;
  virtual double &Timestep(void) = 0;
  virtual double CurrentTime(void) const = 0;
  virtual double SimulationTime(void) const = 0;
  virtual unsigned int GenerationQuantity(void) const = 0;
  virtual unsigned int Generation(void) const = 0;
  virtual unsigned int Language(void) const = 0;

  virtual double AvgFitness(void) const = 0;
  virtual double MaxFitness(void) const = 0;
  virtual double AvgPoints(void) const = 0;
  virtual double AvgSprings(void) const = 0;
  virtual double AvgHeight(void) const = 0;
  virtual double MaxHeight(void) const = 0;

  virtual double  GravityX(void) const = 0;
  virtual double &GravityX(void) = 0;
  virtual double  GravityY(void) const = 0;
  virtual double &GravityY(void) = 0;
  virtual double  GravityZ(void) const = 0;
  virtual double &GravityZ(void) = 0;
  virtual double  Drag(void) const = 0;
  virtual double &Drag(void) = 0;

private:
  iInteractiveEnvironment(const iInteractiveEnvironment &);
  iInteractiveEnvironment &operator=(const iInteractiveEnvironment &);
};

#endif
