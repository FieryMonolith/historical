/***********************************
   Art Covert -- 9/2004
   HFC population storage class for WhirlingDervish
 ***********************************/

#include <algorithm>
#include <cmath>

#include "environment_data.h"
#include "creature_info.h"

#include "hfc_population_storage.h"

using namespace std;

hfcPopulationStorage::hfcPopulationStorage(void)
{
  return;
}

hfcPopulationStorage::~hfcPopulationStorage(void) {
  return;
}

bool hfcPopulationStorage::InitializeLocal(unsigned int, EnvironmentData&, int)
{
  return true;
}

void hfcPopulationStorage::ClearLocal(EnvironmentData &)
{
  return;
}

void hfcPopulationStorage::InitializeADT(void)
{
  ADTs.at(0) = (double)INT_MIN;
  ADTs.at(1) = (double)INT_MAX;
  ADTs.at(2) = (double)INT_MAX;
  ADTs.at(3) = (double)INT_MAX;

  return;
}

void hfcPopulationStorage::DetermineADTValues(vector<double> &fitnessSpread,
					      double fMax, double fAvg,
					      double fStdDev)
{
  //from Hu et al, 
  ADTs.at(0) = (double)INT_MIN;
  ADTs.at(1) = fAvg;
  ADTs.at(2) = ((fMax - fAvg)/2.0) + fAvg;
  ADTs.at(3) = fMax - fStdDev;

  return;
}

void hfcPopulationStorage::PreprocessAllCreatures(EnvironmentData &)
{
  return;
}

void hfcPopulationStorage::HandleOverflow(vector<unsigned int> &idsNotAdded,
					  EnvironmentData &ed)
{
  // Work from highest ids to smallest
  sort(idsNotAdded.begin(), idsNotAdded.end());
  for(unsigned int i=idsNotAdded.size()-1; i>=0; i--)
  {
    // Clear and reassign pointer
    CreatureInfo *ci = allCreatures.at(idsNotAdded.at(i));
    ci->Clean(ed);
    extraCritters.push_back(ci);

    // Remove pointer from allCreatures
    allCreatures.at(idsNotAdded.at(i)) = allCreatures.back();
    allCreatures.pop_back();
  }

  return;
}
