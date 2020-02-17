#ifndef __POPULATION_STORAGE_H__
#define __POPULATION_STORAGE_H__

#include "ipopulation_storage.h"

class PopulationStorage : public iPopulationStorage {
public:
  PopulationStorage(void);
  ~PopulationStorage(void);

  bool Initialize(unsigned int size, EnvironmentData &);
  bool Resize(unsigned int size, EnvironmentData &);
  void Clear(EnvironmentData &);

  unsigned int PopulationSize(void) const { return d_populationSize; }

  const std::vector<CreatureInfo *> &Population(void) const;
        std::vector<CreatureInfo *> &Population(void);

  bool Replace(int64 id, int64 &newId, const std::string &,
	       EnvironmentData &, LanguageEngine &);
  void ReplaceEmpty(int64 id, EnvironmentData &);
  bool Reproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);

  bool Ids(std::vector<int64> &out) const;

private:
  std::vector<CreatureInfo *> d_a;
  std::vector<CreatureInfo *> d_b;

  int d_which;
  unsigned int d_populationSize;
  int64 d_currentId;

  std::vector<CreatureInfo *> &OtherPopulation(void);

  void Fitness(const std::vector<CreatureInfo *> &,
	       std::vector<double> &) const;
  void NormalizedFitness(const std::vector<CreatureInfo *> &,
			 std::vector<double> &) const;
};

#endif
