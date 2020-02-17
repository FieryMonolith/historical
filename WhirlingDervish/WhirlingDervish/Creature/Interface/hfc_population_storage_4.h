#ifndef __HFC_POPULATION_STORAGE_4_H__
#define __HFC_POPULATION_STORAGE_4_H__

#include <fstream>

#include "hfc_population_storage_base.h"

class hfcPopulationStorage4 : public hfcPopulationStorageBase
{
public:
  hfcPopulationStorage4(void);
  ~hfcPopulationStorage4(void);

protected:
  bool InitializeLocal(unsigned int, EnvironmentData &);
  void ClearLocal(EnvironmentData &);
  void InitializeADT(void);
  void DetermineADTValues(std::vector<double> &, double fMax, double fAvg,
			  double fStdDev);
  void PreprocessAllCreatures(EnvironmentData &);
  void HandleOverflow(std::vector<unsigned int> &idsNotAdded,
		      EnvironmentData &);

  //remove obsolete creatures from the buffers
  void cleanBuffers(EnvironmentData&);

  //use the buffer to fill subpopulations
  void useBuffers(EnvironmentData&);

private:
  std::ofstream out;
  std::vector<std::vector<CreatureInfo *> > d_buffer;
  std::vector<unsigned int>  d_levelSize;

};

#endif
