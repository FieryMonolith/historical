#ifndef __HFC_POPULATION_STORAGE_BASE_H__
#define __HFC_POPULATION_STORAGE_BASE_H__

#include "ipopulation_storage.h"

#if 0
#define debug
#endif

class hfcPopulationStorageBase : public iPopulationStorage 
{
public:
  hfcPopulationStorageBase(void);
  ~hfcPopulationStorageBase(void);

  bool Initialize(unsigned int size, EnvironmentData &);
  void Clear(EnvironmentData &);

  bool Reproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);

  unsigned int PopulationSize(void) const;
  const std::vector<CreatureInfo *> &Population(void) const;
        std::vector<CreatureInfo *> &Population(void);
  bool Ids(std::vector<int64> &out) const;

  // Resize may need more work, but we don't use it right now
  bool Resize(unsigned int, EnvironmentData &);
  bool Replace(int64 id, int64& newId, const std::string &str,
	       EnvironmentData &ed, LanguageEngine &le);
  void ReplaceEmpty(int64 id, EnvironmentData &);

protected:
  unsigned int d_Generation;
  int d_currentID;

  std::vector< std::vector<CreatureInfo *> > d_levels;
  std::vector<CreatureInfo *> allCreatures;
  std::vector<CreatureInfo *> extraCritters;
  std::vector<double> ADTs;

  unsigned int d_populationSize;
  unsigned int d_numberOfLevels;
  int64 d_currentId;
  unsigned int d_startHFC;
  unsigned int d_recomputeADTFrequency;

  virtual bool InitializeLocal(unsigned int size, EnvironmentData &)=0;
  virtual void ClearLocal(EnvironmentData &)=0;
  virtual void InitializeADT(void)=0;
  virtual void DetermineADTValues(std::vector<double> &fitnessSpread,
				  double fMax, double fAvg, double fStdDev)=0;
  virtual void PreprocessAllCreatures(EnvironmentData &)=0;
  virtual void HandleOverflow(std::vector<unsigned int> &idsNotAdded,
			      EnvironmentData &ed)=0;

  double stdDev(std::vector<double>&);
  double average(std::vector<double>&);

  //Take allCreatures and divide them into the heirarchy
  void divideAllCreatures(EnvironmentData &);

  // Compute new thresholds based on the fitness of all the current creatures
  void computeADT(std::vector<double>&);
  void fitness(std::vector<double>&);
  void getFitnessSpread(std::vector<CreatureInfo *>&, std::vector<double>&);
  void getNormalizedFitness(std::vector<CreatureInfo*>& critters,
			    std::vector<double>& fitnessSpread);


  //Clear subpopulations
  void clearSubpopulations(EnvironmentData &);

  //Move old critters to extra, clear subpopulations
  void cleanSubpopulations(EnvironmentData &);

  CreatureInfo *GenerateRandom(RandomNumberGenerator &, LanguageEngine &,
			       EnvironmentData &,
			       const std::vector<CreatureInfo *> &critters,
			       const std::vector<double> &fitnessSpread);
  unsigned int getSubPop(CreatureInfo &) const;
};

#endif
