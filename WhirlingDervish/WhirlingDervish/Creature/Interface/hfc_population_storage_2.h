#ifndef __HFC_POPULATION_STORAGE_2_H__
#define __HFC_POPULATION_STORAGE_2_H__

#include "hfc_population_storage_base.h"
#include <fstream>

class hfcPopulationStorage2 : public hfcPopulationStorageBase
{
public:
  hfcPopulationStorage2(void);
  ~hfcPopulationStorage2(void);

  void Clear(EnvironmentData &);
  bool Initialize(unsigned int size,EnvironmentData &,int language);
  bool Reproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);

protected:
  void computeADT(std::vector<double>&);
  //Take allCreatures and divide them into the heirarchy
  void divideAllCreatures(EnvironmentData& ed);
  //Shuffle the createres after they are simulated
  void shuffleAllCreatures( RandomNumberGenerator &rng );

  std::vector<CreatureInfo *> d_temp;
  std::ofstream out;

  std::vector<CreatureInfo *> d_buffer;
  std::vector<int> d_subpopsize;
  std::vector<double> d_subpopCoef;
  std::vector<int> d_effectiveSubPopSize;
  std::vector<CreatureInfo *> d_bufferExcess;
  unsigned int d_bufferMax;
};

#endif
