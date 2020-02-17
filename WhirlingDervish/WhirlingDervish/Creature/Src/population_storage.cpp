#include <algorithm>

#include "random.h"
#include "environment_data.h"
#include "creature_info.h"
#include "language_engine.h"
#include "spider.h"
#include "turtle.h"
#include "parser.h"

#include "population_storage.h"

using namespace std;

PopulationStorage::PopulationStorage(void) : d_which(0),
					     d_populationSize(0),
					     d_currentId(1) {
  return;
}

PopulationStorage::~PopulationStorage(void) {
  return;
}

bool PopulationStorage::Initialize(unsigned int size, EnvironmentData &ed) {
  // This class is considered uninitialized if d_populationSize is zero.
  // If it is non-zero then we assume it is already initialized and we
  // don't want to do that again, use Resize.
  if(d_populationSize != 0) { return true; }

  d_populationSize = size;
  if(d_populationSize == 0) {
    return true;
  }

  d_a.reserve(d_populationSize);
  d_b.reserve(d_populationSize);

  for(unsigned int i=0; i<d_populationSize; i++) {
    CreatureInfo *ci = new CreatureInfo;
    if(ci == NULL) {
      Clear(ed);
      return false;
    }

    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Id() = d_currentId;
    d_currentId++;
    d_a.push_back(ci);

    ci = new CreatureInfo;
    if(ci == NULL) {
      Clear(ed);
      return false;
    }

    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    // These ids don't matter because when you create the next generation
    // these will all be overwritten and give new ids
    ci->Id() = d_currentId;
    d_b.push_back(ci);
  }

  return true;
}

bool PopulationStorage::Resize(unsigned int newPopulationSize,
			       EnvironmentData &ed) {
  if(newPopulationSize == d_populationSize) { return true; }

  if(d_populationSize == 0) {
    return Initialize(newPopulationSize, ed);
  }

  if(d_populationSize > newPopulationSize) {
    for(unsigned int i=0; i<d_populationSize - newPopulationSize; i++) {
      CreatureInfo *ci;

      ci = d_a.back();
      d_a.pop_back();
      if(ci != NULL) {
	ci->Clean(ed);
	delete ci;
      }

      ci = d_b.back();
      d_b.pop_back();
      if(ci != NULL) {
	ci->Clean(ed);
	delete ci;
      }
    }
  } else {
    d_a.reserve(newPopulationSize);
    d_b.reserve(newPopulationSize);

    for(unsigned int i=d_populationSize; i<newPopulationSize; i++) {
      CreatureInfo *ci = new CreatureInfo;
      if(ci == NULL) {
	Clear(ed);
	return false;
      }

      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Id() = d_currentId;
      d_currentId++;
      d_a.push_back(ci);

      ci = new CreatureInfo;
      if(ci == NULL) {
	Clear(ed);
	return false;
      }

      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Id() = d_currentId;
      d_currentId++;
      d_b.push_back(ci);
    }
  }

  d_populationSize = newPopulationSize;

  return true;
}

void PopulationStorage::Clear(EnvironmentData &ed) {
  d_which = 0;
  d_populationSize = 0;

  vector<CreatureInfo *>::iterator i;
  for(i=d_a.begin(); i!=d_a.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }
  d_a.clear();

  for(i=d_b.begin(); i!=d_b.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }
  d_b.clear();

  return;
}

const vector<CreatureInfo *> &PopulationStorage::Population(void) const {
  if(d_which == 0) {
    return d_a;
  }

  return d_b;
}

vector<CreatureInfo *> &PopulationStorage::Population(void) {
  if(d_which == 0) {
    return d_a;
  }

  return d_b;
}

vector<CreatureInfo *> &PopulationStorage::OtherPopulation(void) {
  if(d_which == 0) {
    return d_b;
  }

  return d_a;
}

bool PopulationStorage::Replace(int64 id, int64 &newId, const string &str,
				EnvironmentData &ed,
				LanguageEngine &le) {
  if(d_populationSize == 0) { return true; }

  vector<CreatureInfo *>::iterator i;

  vector<CreatureInfo *> &population = Population();
  for(i=population.begin(); i!=population.end(); ++i) {
    if(*i == NULL) { continue; }
    CreatureInfo *ci = *i;

    if(ci->Id() == id) {
      ci->Clean(ed);

      vector<int> errors;
      if(!le.GetParser().Parse(ci->GetCreature().GetGenome(), errors, str) ||
	 !errors.empty()) {
	ci->Clean(ed);
	return false;
      }
      if(!le.GetSpider().Run(ci->GetCreature().GetGenome(),
			     ci->GetCreature().GetBody(),
			     ed)) {
	ci->Clean(ed);
	return false;
      }
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Pause() = false;
      ci->Still() = false;
      ci->Id() = d_currentId;

      d_currentId++;

      newId = ci->Id();

      return true;
    }
  }

  return false;
}

void PopulationStorage::ReplaceEmpty(int64 id, EnvironmentData &ed) {
  vector<CreatureInfo *>::iterator i;

  vector<CreatureInfo *> &population = Population();
  for(i=population.begin(); i!=population.end(); ++i) {
    if(*i == NULL) { continue; }

    if((*i)->Id() == id) {
      (*i)->Clean(ed);

      return;
    }
  }

  return;
}

bool PopulationStorage::Reproduce(EnvironmentData &ed,
				  RandomNumberGenerator &rng,
				  LanguageEngine &le) {
  if(d_populationSize == 0) { return true; }
  // Should be the same size, also should not be empty if population size is
  // not zero, and should be the same size as d_populationSize
  if(d_a.empty() || d_b.empty() ||
     d_a.size() != d_b.size() ||
     d_a.size() != d_populationSize) {
    Clear(ed);
    return false;
  }

  // Swap the populations, because the other methods use the current population
  // for their operations.
  d_which = (d_which + 1) % 2;

  vector<CreatureInfo *> &oldPopulation = OtherPopulation();
  vector<CreatureInfo *> &newPopulation = Population();
  // Sort the population from highest to lowest
  // Sort creatures from highest to lowest fitness.  There are two reasons
  // for the sort: 1) prevent "bad" zero fitness creatures from replicating
  // 2) ease the search of the file for genotypes based on fitness since
  // genotypes are printed out in the order of the vector
  sort(oldPopulation.begin(), oldPopulation.end(), CreatureInfo::Greater);

  vector<int> originalIds;
  vector<int> repoCounter;
  vector<double> originalFitness;
  vector<CreatureInfo *>::iterator t;
  for(t=oldPopulation.begin(); t!=oldPopulation.end(); ++t) {
    originalFitness.push_back((*t)->Fitness());
    repoCounter.push_back(0);
    originalIds.push_back((*t)->Id());
  }

  // Generates a fitness range used for reproduction, each value corresponds
  // to the creature in the oldPopulation
  vector<double> fitnessRange;
  //Fitness(oldPopulation, fitnessRange);
  NormalizedFitness(oldPopulation, fitnessRange);
  if(fitnessRange.size() != oldPopulation.size()) {
    Clear(ed);
    return false;
  }

  // Find first max fitness, could be more than one if any of the fitnesses
  // were zero
  double testFitness = 0.0;
  unsigned int maxIndex = fitnessRange.size() - 1;
  for(unsigned int j=0; j<fitnessRange.size(); j++) {
    if(fitnessRange[j] > testFitness) {
      testFitness = fitnessRange[j];
      maxIndex = j;
    }
  }

  double maxFitness = fitnessRange.back();

  bool first = true;
  vector<CreatureInfo *>::iterator i;
  for(i=newPopulation.begin(); i!=newPopulation.end(); ++i) {
    if(*i == NULL) {
      Clear(ed);
      return false;
    }

    // Default to the last one in case the rng has a precision error when
    // when comparing to the last fitness in the list
    unsigned int index = maxIndex;
    if(first) {
      index = 0;
    } else {
      double chance = rng.GetDouble() * maxFitness;

      for(unsigned int j=0; j<=maxIndex; j++) {
	if(chance < fitnessRange[j]) {
	  index = j;
	  break;
	}
      }
    }

    repoCounter[index]++;

    CreatureInfo *oci = oldPopulation[index];
    CreatureInfo *ci = *i;
    ci->Clean(ed);

    Spider &spider = le.GetSpider();
    Turtle &turtle = le.GetTurtle();

    if(first) {
      first = false;
      if(!oci->GetCreature().CopyReproduction(ci->GetCreature(), ed,
					      spider, turtle)) {
	Clear(ed);
	return false;
      }
    } else {
      if(!oci->GetCreature().AsexualReproduction(ci->GetCreature(), ed,
						 spider, turtle)) {
	Clear(ed);
	return false;
      }
    }

    ci->Fitness() = 0.0;
    ci->ParentId() = oci->Id();
    ci->Pause() = false;
    ci->Still() = false;
    ci->Id() = d_currentId;
    d_currentId++;
  }

  return true;
}

void PopulationStorage::Fitness(const vector<CreatureInfo *> &in,
				vector<double> &fitnessRange) const {
  double summedFitness = 0;

  vector<CreatureInfo *>::const_iterator i;
  for(i=in.begin(); i!=in.end(); ++i) {
    if(*i != NULL) {
      summedFitness += (*i)->Fitness();
      fitnessRange.push_back(summedFitness);
    }
  }

  // If all the values are zero, make them all one and repeat this function.
  if(!fitnessRange.empty() && fitnessRange.back() == 0) {
    vector<double>::iterator ifr;
    for(ifr=fitnessRange.begin(); ifr!=fitnessRange.end(); ++ifr) {
      *ifr = 1.0;
    }
    Fitness(in, fitnessRange);
  }

  return;
}

void PopulationStorage::NormalizedFitness(const vector<CreatureInfo *> &in,
					  vector<double> &fitnessRange) const {
  double maxFitness = 0;

  // Fill in the fitnessRange vector with the corresponding fitness values
  vector<CreatureInfo *>::const_iterator i;
  for(i=in.begin(); i!=in.end(); ++i) {
    if(*i == NULL) { continue; }

    double fitness = (*i)->Fitness();
    if(fitness > maxFitness) {
      maxFitness = fitness;
    }
    fitnessRange.push_back(fitness);
  }

  // Either set them all to zero, if they were all zero, or normalize them
  vector<double>::iterator ii;
  if(maxFitness == 0) {
    for(ii=fitnessRange.begin(); ii!=fitnessRange.end(); ++ii) {
      *ii = 1.0;
    }
  } else {
    for(ii=fitnessRange.begin(); ii!=fitnessRange.end(); ++ii) {
      *ii /= maxFitness;
    }
  }

  double summedFitness = 0;
  for(ii=fitnessRange.begin(); ii!=fitnessRange.end(); ++ii) {
    summedFitness += *ii;
    *ii = summedFitness;
  }

  return;
}

bool PopulationStorage::Ids(vector<int64> &out) const {
  out.clear();

  if(d_populationSize == 0) { return true; }

  out.reserve(d_populationSize);

  const vector<CreatureInfo *> &population = Population();
  vector<CreatureInfo *>::const_iterator i;

  for(i=population.begin(); i!=population.end(); ++i) {
    if(*i == NULL) {
      return false;
    }

    out.push_back((*i)->Id());
  }

  return true;
}
