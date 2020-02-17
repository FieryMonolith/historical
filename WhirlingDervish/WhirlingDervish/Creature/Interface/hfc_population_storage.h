#ifndef __HFC_POPULATION_STORAGE_H__
#define __HFC_POPULATION_STORAGE_H__

#include "hfc_population_storage_base.h"

class hfcPopulationStorage : public hfcPopulationStorageBase
{
public:
  hfcPopulationStorage(void);
  ~hfcPopulationStorage(void);

protected:
  bool InitializeLocal(unsigned int, EnvironmentData &, int);
  void ClearLocal(EnvironmentData &);
  void InitializeADT(void);
  void DetermineADTValues(std::vector<double> &, double fMax, double fAvg,
			  double fStdDev);
  void PreprocessAllCreatures(EnvironmentData &);
  void HandleOverflow(std::vector<unsigned int> &idsNotAdded,
		      EnvironmentData &);
};

#endif
