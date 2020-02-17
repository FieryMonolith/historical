#ifndef __HFC_POPULATION_STORAGE_2_H__
#define __HFC_POPULATION_STORAGE_2_H__

#include "ipopulation_storage.h"
#include <fstream>

class hfcPopulationStorage : public iPopulationStorage 
{
public:
  hfcPopulationStorage(void);
  ~hfcPopulationStorage(void);
  #if 1
  void Clear(EnvironmentData &);

  bool Initialize(unsigned int size,EnvironmentData &,int language);
  //bool SetPopulationSize(int, EnvironmentData &);
  bool Resize(unsigned int, EnvironmentData &, int language);

  unsigned int PopulationSize(void) const;

  const std::vector<CreatureInfo *> &Population(void) const;
        std::vector<CreatureInfo *> &Population(void);

  bool Replace(int64 id, int64& newId, const std::string &str,
       EnvironmentData &ed, LanguageEngine &le, int language);

  //bool Fill(EnvironmentData &, int language);
  bool Reproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);
  void ReplaceEmpty(int64 id, EnvironmentData &);

  bool Ids(std::vector<int64> &out) const;

private:

#endif
  int d_Generation;
  int d_calibGen;
  int d_currentID;
  int d_lang;

#if 1
  double fMax, fAvg, fStdDev;

  double stdDev(std::vector<double>&);
  double average(std::vector<double>&);

  void fitness(std::vector<double>&);
  void getFitnessSpread(std::vector<CreatureInfo *>&, std::vector<double>&);
  void getNormalizedFitness(std::vector<CreatureInfo*>& critters, std::vector<double>& fitnessSpread);
  void computeADT(std::vector<double>&);

  //Take all creatures out of the heirarchy and place them in allCreatures
  void getAllCreatures();

  //get the integer of the subpopulation this organism should go into
  int getSubPop(CreatureInfo* ci);

  //Take allCreatures and divide them into the heirarchy
  void divideAllCreatures(EnvironmentData& ed);

  //Move old critters to extra, clear subpopulations
  void clearSubpopulation(EnvironmentData &);
  void clearSubpopulationb(EnvironmentData &);

  //Delete all creatures in the subpopulations
  void deleteSubpopulation(EnvironmentData &);

  //Shuffle the createres after they are simulated
  void shuffleAllCreatures( RandomNumberGenerator &rng );

  bool subpopReproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);

  std::vector< std::vector<CreatureInfo *> > d_a;
  std::vector< std::vector<CreatureInfo *> > d_b;
  std::vector<CreatureInfo *> d_temp;

  std::ofstream out;

  std::vector<std::vector<CreatureInfo *> > d_buffer;
  
  std::vector<CreatureInfo *> allCreatures;
  std::vector<CreatureInfo *> allCreatures_b;

  std::vector<CreatureInfo *> extraCritters;

  std::vector<double> ADTs;
  std::vector<int> d_subpopsize;
  std::vector<double> d_subpopCoef;
  std::vector<int> d_effectiveSubPopSize;

  int d_populationSize;

  int64 d_currentId;

  unsigned critterCount;

  const int &Count(void) const;
        int &Count(void);
#endif
};

#endif
