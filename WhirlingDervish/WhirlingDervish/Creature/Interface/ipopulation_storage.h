#ifndef __IPOPULATION_STORAGE_H__
#define __IPOPULATION_STORAGE_H__

#include <vector>
#include <string>

#include "defs.h"

class CreatureInfo;
class EnvironmentData;
class RandomNumberGenerator;
class LanguageEngine;

class iPopulationStorage {
public:
  iPopulationStorage(void) { return; }
  // This does not have to clean up, unless clean does more than the Clear
  // function.  The environment will call the Clear function when it 
  // terminates because you need the EnvironmentData to delete stuff correctly
  virtual ~iPopulationStorage(void) { return; }

  // Create a population of empty organisms, of size size
  virtual bool Initialize(unsigned int size, EnvironmentData &)=0;
  // Removes extras or fills in creatures if there isn't enough
  // for the new size
  virtual bool Resize(unsigned int size, EnvironmentData &)=0;
  // the int is the new size of the population
  // Clear the population, leaving it empty
  virtual void Clear(EnvironmentData &)=0;

  // Get the number of entries in the vector of CreatureInfo *
  virtual unsigned int PopulationSize(void) const = 0;

  virtual const std::vector<CreatureInfo *> &Population(void) const = 0;
  virtual       std::vector<CreatureInfo *> &Population(void)       = 0;

  // Finds the creature with id, creates a new creature using the same
  // data structure but with the new genome string, and gives it a new
  // id
  virtual bool Replace(int64 id, int64 &newId, const std::string &,
		       EnvironmentData &, LanguageEngine &)=0;
  // Empties the creature with id
  virtual void ReplaceEmpty(int64 id, EnvironmentData &)=0;
  // Create a new population of organisms using the current population
  virtual bool Reproduce(EnvironmentData &, RandomNumberGenerator &,
			 LanguageEngine &)=0;

  // Input a vector and it is filled with the ids of all the entities in
  // the population
  virtual bool Ids(std::vector<int64> &out) const=0;
};

#endif
