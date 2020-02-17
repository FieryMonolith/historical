#ifndef _HFC_POPULATION_STORAGE_6_H_
#define _HFC_POPULATION_STORAGE_6_H_

#include <fstream>

#include "hfc_population_storage_base.h"
#include "cladeMap.h"


class hfcPopulationStorage6 : public hfcPopulationStorageBase
{
 public:
     hfcPopulationStorage6(void);
     ~hfcPopulationStorage6(void);

     virtual bool Initialize(unsigned int size, EnvironmentData &);
     virtual void Clear(EnvironmentData &);
     virtual bool Reproduce(EnvironmentData &, RandomNumberGenerator &, LanguageEngine &);

     virtual       vector<CreatureInfo*>& Population(void);
     virtual const vector<CreatureInfo*>& Population(void) const;
     
     
 private:

     unsigned int d_numberOfLevels;
     unsigned int d_currLevels;
     unsigned int d_levelSize;
     unsigned int d_Generation;
     //int64 d_currentID;
     unsigned int d_clade;
     
     std::vector<CreatureInfo *> d_newCritters;
     std::vector<unsigned> d_traitSelection;
     std::vector<CreatureInfo *> allCreatures;
     std::vector<CreatureInfo *> extraCritters;
     std::vector<double> ADTs;

     unsigned int d_populationSize;

     int64 d_currentId;
     unsigned int d_startHFC;
     unsigned int d_recomputeADTFrequency;

     unsigned int d_fMax;
     unsigned int d_fMin;
     
     virtual bool InitializeLocal(unsigned int size, EnvironmentData &);
     virtual void ClearLocal(EnvironmentData &);
     virtual void InitializeADT(void);
     virtual void DetermineADTValues(std::vector<double> &fitnessSpread,
                                     double fMax, double fAvg, double fStdDev);
     virtual void PreprocessAllCreatures(EnvironmentData &);
     virtual void HandleOverflow(std::vector<unsigned int> &idsNotAdded,
                                 EnvironmentData &ed);
     

     bool isin(std::vector<unsigned>&, unsigned);
     void getCandidates(unsigned, unsigned, std::vector<CreatureInfo*>&);
     //double stdDev(std::vector<double>&);
     //double average(std::vector<double>&);
     
     //Take allCreatures and divide them into the heirarchy
     void divideAllCreatures(EnvironmentData &);
     
     //Clear subpopulations
     void clearSubpopulations(EnvironmentData &);
     
     //Move old critters to extra, clear subpopulations
     void cleanSubpopulations(EnvironmentData &);
     
     CreatureInfo *GenerateRandom(RandomNumberGenerator &, LanguageEngine &,
                                  EnvironmentData &,
                                  const std::vector<CreatureInfo *> &critters,
                                  const std::vector<double> &fitnessSpread);

     unsigned int getSubPop(CreatureInfo *) const;

     ofstream cladeOut;
     ofstream replicateOut;
};

#endif
