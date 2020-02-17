#include <iostream>

#include <vector>
#include <map>

#include "jvector.h"
#include "creature_info.h"

#include "interactive_environment.h"

#include "environment_data.h"
#include "file_handler.h"
#include "ipopulation_storage.h"
#include "population_storage.h"
#include "body.h"
#include "creature.h"
#include "physical_point.h"
#include "physical_point_info.h"
#include "spring.h"
#include "spring_info.h"
#include "population_storage.h"
#include "language_engine.h"
#include "parser.h"
#include "spider.h"

using namespace std;

InteractiveEnvironment::InteractiveEnvironment(int id) : d_id(id) {
  return;
}

InteractiveEnvironment::~InteractiveEnvironment(void) {
  return;
}

bool InteractiveEnvironment::Initialize(void) {
  return Environment::Initialize();
}

bool InteractiveEnvironment::LoadConfiguration(FileHandler &fh,
				       const string &runSettingsFilename,
				       const string &physicsSettingsFilename,
				       const string &ecSettingsFilename,
				       const string &instructionSetFilename) {
  return Environment::LoadConfiguration(fh,
					runSettingsFilename,
					physicsSettingsFilename,
					ecSettingsFilename,
					instructionSetFilename);
}

bool InteractiveEnvironment::InitializeExperiment(FileHandler &fh) {
  return Environment::InitializeExperiment(fh);
}

void InteractiveEnvironment::Shutdown(FileHandler &fh) {
  Environment::Shutdown(fh);

  return;
}

bool InteractiveEnvironment::Process(double timestep) {
  if(d_pause) { return true; }
  return ProcessSimulationStep(timestep);
}

bool InteractiveEnvironment::ExperimentProcess(double timestep,
					       FileHandler &fh) {
  if(d_pause) { return true; }
  return ProcessGenerationStep(timestep, d_currentTime,
			       d_physicsSettings.Simtime(),
			       d_generation, fh, OutputFileId());
}

int InteractiveEnvironment::CurrentGeneration(void) const {
  return Generation();
}

int InteractiveEnvironment::MaxGeneration(void) const {
  return GenerationQuantity();
}

bool InteractiveEnvironment::Paused(void) {
  return d_pause;
}

void InteractiveEnvironment::Pause(void) {
  d_pause = true;

  return;
}

void InteractiveEnvironment::Unpause(void) {
  d_pause = false;

  return;
}

void InteractiveEnvironment::SelectCreature(int64 id) {
  d_selected = true;
  d_selectedId = id;

  return;
}

void InteractiveEnvironment::DeselectCreature(void) {
  d_selected = false;
  d_selectedId = 0;

  return;
}

void InteractiveEnvironment::DeleteSelected(void) {
  if(!d_selected) {
    return;
  }

  DeleteCreature(d_selectedId);

  return;
}

void InteractiveEnvironment::DeleteCreature(int64 id) {
  d_populationStorage.ReplaceEmpty(id, (EnvironmentData &)(*this));

  return;
}

void InteractiveEnvironment::ResetSelected(void) {
  if(!d_selected) {
    return;
  }

  ResetCreature(d_selectedId);

  return;
}

void InteractiveEnvironment::ResetCreature(int64 id) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return; }

  ci->BodyReset();

  return;
}

void InteractiveEnvironment::MovePoint(int64 id, const Vector3d &shift) {
  PhysicalPointInfo *ppi = d_shapeEngine.GetPointInfo(id);
  if(ppi == NULL) { return; }

  ppi->GetPhysicalPoint().Position() += shift;

  return;
}

void InteractiveEnvironment::MoveLine(int64 point1Id, int64 point2Id, 
				      const Vector3d &shift) {
  PhysicalPointInfo *ppi1 = d_shapeEngine.GetPointInfo(point1Id);
  if(ppi1 == NULL) { return; }

  PhysicalPointInfo *ppi2 = d_shapeEngine.GetPointInfo(point2Id);
  if(ppi2 == NULL) { return; }

  ppi1->GetPhysicalPoint().Position() += shift;
  ppi2->GetPhysicalPoint().Position() += shift;

  return;
}

void InteractiveEnvironment::MoveSelected(const Vector3d &shift) {
  if(!d_selected) { return; }

  MoveCreature(d_selectedId, shift);

  return;
}

void InteractiveEnvironment::MoveCreature(int64 id, const Vector3d &shift) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return; }

  Body &b = ci->GetCreature().GetBody();

  const map<int64, PhysicalPointInfo *> &points = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;
  for(ip=points.begin(); ip!=points.end(); ++ip) {
    if(ip->second == NULL) { continue; }

    ip->second->GetPhysicalPoint().Position() += shift;
    if(ip->second->GetPhysicalPoint().Position()[1] < 0) {
      ip->second->GetPhysicalPoint().Position()[1] = 0;
    }
  }

  return;
}

void InteractiveEnvironment::FreezePoint(int64 id) {
  PhysicalPointInfo *ppi = d_shapeEngine.GetPointInfo(id);
  if(ppi == NULL) { return; }

  ppi->Pause() = true;

  return;
}

void InteractiveEnvironment::FreezeLine(int64 point1Id, int64 point2Id,
					int64 springId) {
  PhysicalPointInfo *ppi1 = d_shapeEngine.GetPointInfo(point1Id);
  if(ppi1 == NULL) { return; }

  PhysicalPointInfo *ppi2 = d_shapeEngine.GetPointInfo(point2Id);
  if(ppi2 == NULL) { return; }

  SpringInfo *si = d_shapeEngine.GetSpringInfo(springId);
  if(si == NULL) { return; }

  ppi1->Pause() = true;
  ppi2->Pause() = true;
  si->Pause() = true;

  return;
}

void InteractiveEnvironment::FreezeSelected(void) {
  if(!d_selected) { return; }

  FreezeCreature(d_selectedId);

  return;
}

void InteractiveEnvironment::FreezeCreature(int64 id) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return; }

  Body &b = ci->GetCreature().GetBody();

  const map<int64, PhysicalPointInfo *> &points = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;
  for(ip=points.begin(); ip!=points.end(); ++ip) {
    if(ip->second == NULL) { continue; }

    ip->second->Pause() = true;
  }

  const map<int64, SpringInfo *> &springs = b.GetSprings();
  map<int64, SpringInfo *>::const_iterator is;
  for(is=springs.begin(); is!=springs.end(); ++is) {
    if(is->second == NULL) { continue; }

    is->second->Pause() = true;
  }

  return;
}

void InteractiveEnvironment::UnfreezePoint(int64 id) {
  PhysicalPointInfo *ppi = d_shapeEngine.GetPointInfo(id);
  if(ppi == NULL) { return; }

  ppi->Pause() = false;

  return;
}

void InteractiveEnvironment::UnfreezeLine(int64 point1Id, int64 point2Id,
					  int64 springId) {
  PhysicalPointInfo *ppi1 = d_shapeEngine.GetPointInfo(point1Id);
  if(ppi1 == NULL) { return; }

  PhysicalPointInfo *ppi2 = d_shapeEngine.GetPointInfo(point2Id);
  if(ppi2 == NULL) { return; }

  SpringInfo *si = d_shapeEngine.GetSpringInfo(springId);
  if(si == NULL) { return; }

  ppi1->Pause() = false;
  ppi2->Pause() = false;
  si->Pause() = false;

  return;
}

void InteractiveEnvironment::UnfreezeSelected(void) {
  if(!d_selected) { return; }

  UnfreezeCreature(d_selectedId);

  return;
}

void InteractiveEnvironment::UnfreezeCreature(int64 id) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return; }

  Body &b = ci->GetCreature().GetBody();

  const map<int64, PhysicalPointInfo *> &points = b.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator ip;
  for(ip=points.begin(); ip!=points.end(); ++ip) {
    if(ip->second == NULL) { continue; }

    ip->second->Pause() = false;
  }

  const map<int64, SpringInfo *> &springs = b.GetSprings();
  map<int64, SpringInfo *>::const_iterator is;
  for(is=springs.begin(); is!=springs.end(); ++is) {
    if(is->second == NULL) { continue; }

    is->second->Pause() = false;
  }

  return;
}

bool InteractiveEnvironment::GenomeSelected(ostream &s) const {
  if(!d_selected) { return false; }

  GenomeCreature(d_selectedId, s);

  return true;
}

bool InteractiveEnvironment::GenomeCreature(int64 id, ostream &s) const {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return false; }

  return ci->GetCreature().GetGenome().Stringify(s);
}

bool InteractiveEnvironment::AlterSelected(const string &s) {
  if(!d_selected) { return false; }

  return AlterCreature(d_selectedId, s);
}

bool InteractiveEnvironment::AlterCreature(int64 id, const string &s) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return false; }

  ci->Clean((EnvironmentData &)(*this));

  Parser &p = GetLanguageEngine().GetParser();
  vector<int> errors;
  if(!p.Parse(ci->GetCreature().GetGenome(), errors, s)) { return false; }

  Spider &spider = GetLanguageEngine().GetSpider();
  return spider.Run(ci->GetCreature().GetGenome(),
		    ci->GetCreature().GetBody(),
		    (EnvironmentData &)(*this));
}

bool InteractiveEnvironment::AlterSelected(const std::string &s,
					   double fitness) {
  if(!d_selected) { return false; }

  return AlterCreature(d_selectedId, s, fitness);
}

bool InteractiveEnvironment::AlterCreature(int64 id, const std::string &s,
					   double fitness) {
  CreatureInfo *ci = FindCreatureInfo(id);
  if(ci == NULL) { return false; }

  ci->Fitness() = fitness;

  if(!AlterCreature(id, s)) { return false; }

  return true;
}

bool InteractiveEnvironment::InsertCreature(int64 &id, const string &s) {
  iPopulationStorage &ps = GetPopulationStorage();
  vector<int64> ids;
  if(!ps.Ids(ids) || ids.empty()) {
    return false;
  }
  return ps.Replace(ids[0], id, s, (EnvironmentData &)(*this),
		    GetLanguageEngine());
}

bool InteractiveEnvironment::FillSelectedInfo(CreatureInfo &ci) const {
  if(!d_selected) { return false; }

  return FillCreatureInfo(d_selectedId, ci);
}

bool InteractiveEnvironment::FillCreatureInfo(int64 id,
					      CreatureInfo &ci) const {
  CreatureInfo *pci = FindCreatureInfo(id);
  if(pci == NULL) { return false; }

  ci.Id() = pci->Id();
  ci.ParentId() = pci->ParentId();
  ci.Fitness() = pci->Fitness();
  ci.Pause() = pci->Pause();
  ci.Still() = pci->Still();
  ci.GetCreature().GetGenome().Copy(pci->GetCreature().GetGenome());

  return true;
}

Parser &InteractiveEnvironment::GetParser(void) {
  return GetLanguageEngine().GetParser();
}

iPopulationStorage &InteractiveEnvironment::GetPS(void) {
  return GetPopulationStorage();
}

CreatureInfo *InteractiveEnvironment::FindCreatureInfo(int64 id) const {
  const vector<CreatureInfo *> &population = d_populationStorage.Population();
  vector<CreatureInfo *>::const_iterator i;

  for(i=population.begin(); i!=population.end(); ++i) {
    if(*i == NULL) { continue; }

    if((*i)->Id() == id) {
      break;
    }
  }

  if(i != population.end()) {
    return *i;
  }

  return NULL;
}
