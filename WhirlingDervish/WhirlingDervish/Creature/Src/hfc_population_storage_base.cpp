/***********************************
   Art Covert -- 9/2004
   HFC population storage class for WhirlingDervish
 ***********************************/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "random.h"
#include "environment_data.h"
#include "creature_info.h"
#include "language_engine.h"
#include "spider.h"
#include "turtle.h"
#include "parser.h"

#include "hfc_population_storage_base.h"

//#define debug

using namespace std;

hfcPopulationStorageBase::hfcPopulationStorageBase(void)
  : d_Generation(0),
    d_numberOfLevels(4),
    d_currentId(0),
    d_startHFC(20),
    d_recomputeADTFrequency(5)
{
  return;
}

hfcPopulationStorageBase::~hfcPopulationStorageBase(void) {
  return;
}

bool hfcPopulationStorageBase::Initialize(unsigned int size,
					  EnvironmentData &ed)
{
#ifdef debug
  cerr << "Initalize_top" << endl;
#endif

  Clear(ed);
  d_populationSize = size;
  if(size <= 0) { return true; }

  // create levels for hfc
  // assuming d_numberOfLevels is the number of levels possible
  vector<CreatureInfo*> temp;
  temp.reserve(d_populationSize);

  d_levels.reserve(d_numberOfLevels);
  for(unsigned int i=0; i<d_numberOfLevels; i++){
    d_levels.push_back(temp);
  }

  // Prepare vector memory
  allCreatures.reserve(d_populationSize * d_numberOfLevels);
  extraCritters.reserve(d_populationSize * d_numberOfLevels * 2);

  // Create creatures
  CreatureInfo *ci;
  for(unsigned int i=0; i<d_populationSize * d_numberOfLevels * 2; i++) {
    ci = new CreatureInfo();
    if(ci == NULL) {
      Clear(ed);
      return false;
    }
    ci->Clean(ed);
    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Id() = d_currentId;
    d_currentId++;

    extraCritters.push_back(ci);
  }

  // Add on d_populationSize empty creatures to the first level
  for(unsigned int i=0; i<d_populationSize; i++) {
    ci = extraCritters.back();
    extraCritters.pop_back();

    d_levels.at(0).push_back(ci);
    allCreatures.push_back(ci);
  }

  // Initialize ADT
  ADTs.reserve(d_numberOfLevels);
  for(unsigned int i=0; i<d_numberOfLevels; i++)
  {
    ADTs.push_back(0);
  }
  InitializeADT();

  InitializeLocal(size, ed);

#ifdef debug
  cerr << "Initalize_bottom" << endl;
#endif

  return true;
}

void hfcPopulationStorageBase::Clear(EnvironmentData &ed)
{
#ifdef debug
  cerr << "clear_top" << endl;
#endif

  ClearLocal(ed);

  d_populationSize = 0;

  vector<CreatureInfo *>::iterator i;
  for(i=allCreatures.begin(); i!=allCreatures.end(); ++i) {
    extraCritters.push_back(*i);
  }
  allCreatures.clear();
  //cerr << "AllCreatures clear" << endl;

  // loop through all four levels and clear them
  vector<vector<CreatureInfo *> >::iterator levels;
  for(levels=d_levels.begin(); levels!=d_levels.end(); ++levels) {
    (*levels).clear();
  }
  d_levels.clear();

  //cerr << "Start Clean" << endl;
  /*
    for(i=extraCritters.begin(); i!=extraCritters.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }
  */
  //cerr << "End Clean" << endl;
  extraCritters.clear();

#ifdef debug
  cerr << "clear_bottom" << endl;
#endif

  return;
}

bool hfcPopulationStorageBase::Resize(unsigned int newPopulationSize,
				      EnvironmentData &ed) 
{
#ifdef debug
  cerr << "Resize_top" << endl;
#endif

  if(newPopulationSize == d_populationSize) { return true; }

  CreatureInfo *ci;
  if(newPopulationSize > d_populationSize)
  {
    unsigned int needed = (newPopulationSize - d_populationSize) *
                          d_numberOfLevels;
    for(unsigned int i=0; i<needed; i++){
      ci = new CreatureInfo();
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Id() = d_currentId;
      d_currentId++;

      extraCritters.push_back(ci);
    }
  }
  else if(newPopulationSize < d_populationSize)
  {
    allCreatures.clear();
    
    for(unsigned int i=0; i<d_numberOfLevels; i++)
    {
      unsigned int levelSize = d_levels.at(i).size();
      unsigned int j = 0;
      for(; j < newPopulationSize && j < levelSize; j++)
      {
	allCreatures.push_back(d_levels.at(i).at(j));
      }
      for(; j<levelSize; j++)
      {
	extraCritters.push_back(d_levels.at(i).back());
	d_levels.at(i).pop_back();
      }
    }

    int toRemove = (int)(extraCritters.size() + allCreatures.size()) -
                   (int)(newPopulationSize * d_numberOfLevels * 2);
    if(toRemove < 0)
    {
      // Create
      unsigned int needed = (unsigned int)(toRemove * -1);
      for(unsigned int i=0; i<needed; i++){
	ci = new CreatureInfo();
	ci->ParentId() = 0;
	ci->Fitness() = 0.0;
	ci->Id() = d_currentId;
	d_currentId++;

	extraCritters.push_back(ci);
      }
    }
    else if(toRemove > 0)
    {
      // Destroy
      for(unsigned int i=0; (int)i<toRemove && i<extraCritters.size(); i++)
      {
	ci = extraCritters.back();
	if(ci != NULL) {
	  ci->Clean(ed);
	  delete ci;
	  ci = NULL;
	}
      }
    }
  }

  d_populationSize = newPopulationSize;

#ifdef debug
  cerr << "Resize_bottom" << endl;
#endif

  return true;
}

vector<CreatureInfo*>& hfcPopulationStorageBase::Population() {
  return allCreatures;
}

const vector<CreatureInfo*>& hfcPopulationStorageBase::Population() const {
  return allCreatures;
}

unsigned int hfcPopulationStorageBase::PopulationSize(void) const {
  return d_populationSize;
}

/**********************
 Insert a criter to the base population, will be promoted if necessary
 at the next iteration
 **********************/
bool hfcPopulationStorageBase::Replace(int64 id, int64 &newID,
				       const string &str, EnvironmentData &ed,
				       LanguageEngine &le) {
#ifdef debug
  cerr << "Replace_top" << endl;
#endif

  for(unsigned int i = 0; i < allCreatures.size(); i++) {
    if(allCreatures.at(i)->Id() == id) {
      CreatureInfo *ci = allCreatures.at(i);
      vector<int> errors;
      if(!le.GetParser().Parse(ci->GetCreature().GetGenome(),
			       errors, str) || !errors.empty()) 
      {
	ci->Clean(ed);
	return false;
      }
      if(!le.GetSpider().Run(ci->GetCreature().GetGenome(),
			     ci->GetCreature().GetBody(),
			     ed)) 
      {
	ci->Clean(ed);
	return false;
      }
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Pause() = false;
      ci->Still() = false;
      ci->Id() = d_currentId;
      newID = d_currentId;

      d_currentId++;

      return true;
    }
  }

#ifdef debug
  cerr << "Replace_bottom" << endl;
#endif

  return false;
}

/********************
 Turns an organism into an empty organism with a new id
 ********************/
void hfcPopulationStorageBase::ReplaceEmpty(int64 id, EnvironmentData &ed) {
  for(unsigned int i = 0; i < allCreatures.size(); i++) {
    if(allCreatures.at(i)->Id() == id) {
      CreatureInfo *ci = allCreatures.at(i);
      ci->Clean(ed);
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Pause() = false;
      ci->Still() = false;
      ci->Id() = d_currentId;

      d_currentId++;
    }
  }

  return;
}

/*********************
 Perform reprodcution for a generation
 *********************/
bool hfcPopulationStorageBase::Reproduce(EnvironmentData &ed,
					 RandomNumberGenerator &rng,
					 LanguageEngine &le) {
  d_Generation++;

  if(d_Generation >= d_startHFC && d_Generation % d_recomputeADTFrequency == 0)
  {
    // recompute adaptive thresholds  
    vector<double> fitnessSpread;
    getFitnessSpread(allCreatures, fitnessSpread);
    computeADT(fitnessSpread);
  }

  //divide population into HFC subpopulations
  PreprocessAllCreatures(ed);

  for(unsigned int i=0; i<d_numberOfLevels; i++)
  {
    vector<CreatureInfo *> &critters = d_levels.at(i);

    cerr << "Reproducing population " << i << endl;
    cerr << "subpopulation has : " << critters.size() << " creatures.";
    cerr << endl;
      
    if(critters.empty()) { continue; }

    // Get normalized, sorted fitness of all the creatures at this level
    vector<double> fitnessSpread;
    getNormalizedFitness(critters, fitnessSpread);
      
    unsigned count = 0;
    for(unsigned int size=1; size <= d_populationSize; size++)
    {
      
      CreatureInfo *nci = GenerateRandom(rng, le, ed, critters, fitnessSpread);
      if(nci == NULL)
      {
	Clear(ed);
	return false;
      }
      allCreatures.push_back(nci);
      count ++;
    }
    cerr << "Level " << i << " has " << count << " children." << endl;
  }

  cleanSubpopulations(ed);

#if 1
  // Assuming this means total allocated creatures
  cerr << "critterCount : " << allCreatures.size() + extraCritters.size()
       << endl; 
  cerr << "allCritters  : " << allCreatures.size() << endl;
#endif

  return true;
}

/**************************
compute the Adaptive Threshholds for the next population
**************************/
void hfcPopulationStorageBase::computeADT(vector<double>& fitnessSpread){
  sort(fitnessSpread.begin(), fitnessSpread.end());

  ADTs.clear();
  for(unsigned i=0; i<d_numberOfLevels; i++) ADTs.push_back(0);

  DetermineADTValues(fitnessSpread, fitnessSpread.back(),
		     average(fitnessSpread), stdDev(fitnessSpread));

#if 1
  cout << "Addaptive Thresholds set:" << endl;
  cout << showpoint << fixed << setprecision(6);
  for(unsigned int i=1; i<=ADTs.size(); i++)
  {
    cout << setw(10) << "ADT Level " << i << ": " << ADTs.at(i-1) << endl; 
  }
#endif

  return;
}

bool hfcPopulationStorageBase::Ids(std::vector<int64> &out) const
{
  for(unsigned i = 0; i < allCreatures.size(); i++){
    if(allCreatures.at(i) == NULL) return false;
    out.push_back(allCreatures.at(i)->Id());
  }
  return true;
}

/********************
 get the fitness Spread of the entire population
 ********************/
void hfcPopulationStorageBase::fitness(vector<double>& fitnessSpread){
  fitnessSpread.clear();

  vector<CreatureInfo*>::iterator i;
  for(i = allCreatures.begin(); i != allCreatures.end(); ++i){
    if(*i != NULL)
    {
      fitnessSpread.push_back((*i)->Fitness());
    }
  }

  return;
}

/********************
 get the fitnessSpread for a subuset of the population
 ********************/
void hfcPopulationStorageBase::getFitnessSpread(vector<CreatureInfo*>& critters, vector<double>& fitnessSpread){
  fitnessSpread.clear();
  for(unsigned i = 0; i < critters.size(); i++){
    fitnessSpread.push_back(critters.at(i)->Fitness());
  }

  return;
}

/********************
get the normalized fitnesses

same as Jason's for comparison purposes
********************/
void hfcPopulationStorageBase::getNormalizedFitness(vector<CreatureInfo*>& critters, vector<double>& fitnessSpread){
  fitnessSpread.clear();
 
  sort(critters.begin(), critters.end(), CreatureInfo::Greater);

  vector<CreatureInfo*>::iterator i;
  for(i = critters.begin(); i != critters.end(); i++){
    if(*i != NULL){
      fitnessSpread.push_back((*i)->Fitness());
    }
  }

  double maxFit = fitnessSpread.at(0);

  //if the highest fitness is zero set all fitnesses to one
  if(maxFit == 0){
    for(unsigned j = 0; j < fitnessSpread.size(); j++)
      fitnessSpread.at(j) = 1;
  }else{
    for(unsigned j = 0; j < fitnessSpread.size(); j++){
      fitnessSpread.at(j) /= maxFit;
    }

  }

  maxFit = 0;
  for(unsigned j = 0; j < fitnessSpread.size(); j++){
    maxFit += fitnessSpread.at(j);
    fitnessSpread.at(j) = maxFit;
  }

  return;
}

double hfcPopulationStorageBase::average(vector<double>& fitnessSpread){
  if(fitnessSpread.empty()) { return 0.0; }

  double sum = 0.0;

  for (unsigned i=0; i < fitnessSpread.size(); i++) sum += fitnessSpread.at(i);

  return sum / fitnessSpread.size();
}

double hfcPopulationStorageBase::stdDev(vector<double>& fitnessSpread){
  if(fitnessSpread.empty()) { return 0.0; }
  double avg = average(fitnessSpread);
  double sum = 0.0;

  for(unsigned i = 0; i < fitnessSpread.size(); i++) 
    sum += ((fitnessSpread.at(i) - avg)*(fitnessSpread.at(i) - avg));

  sum = sum / fitnessSpread.size();

  return sqrt(sum);
}

void hfcPopulationStorageBase::divideAllCreatures(EnvironmentData &ed)
{
#if 1
  cerr << "Total population size: " << allCreatures.size() << endl;
#endif

  vector<unsigned int> idsNotAdded;

  for(unsigned int i = 0; i < allCreatures.size(); i++){
    CreatureInfo *ci = allCreatures.at(i);

    int which = 0;
    if(d_numberOfLevels > 1)
    {
      which = ADTs.size() - 1;
      for(unsigned int j=1; j<ADTs.size()-1; j++)
      {
	if(ci->Fitness() < ADTs.at(j))
	{
	  which = j - 1;
	  break;
	}
      }
    }

    if(d_levels.at(which).size() + 1 > d_populationSize) {
      idsNotAdded.push_back(i);
    } else {
      d_levels.at(which).push_back(ci);
    }
  }

  HandleOverflow(idsNotAdded, ed);

#if 1
  cout << "Subpopulation sizes" << endl;
  for(unsigned int i=1; i<=ADTs.size(); i++)
  {
    cout << "Level " << i << ": " << d_levels.at(i-1).size() << endl;
  }
#endif

  return;
}



void hfcPopulationStorageBase::clearSubpopulations(EnvironmentData &ed){
  vector<vector<CreatureInfo *> >::iterator levels;
  for(levels=d_levels.begin(); levels!=d_levels.end(); ++levels) {
    vector<CreatureInfo *> &level = *levels;
    level.clear();
  }

  return;
}


void hfcPopulationStorageBase::cleanSubpopulations(EnvironmentData &ed){
  vector<vector<CreatureInfo *> >::iterator levels;
  for(levels=d_levels.begin(); levels!=d_levels.end(); ++levels) {
    vector<CreatureInfo *> &level = *levels;

    vector<CreatureInfo *>::iterator ilevel;
    for(ilevel=level.begin(); ilevel!=level.end(); ++ilevel) {
      // ERROR
      if(*ilevel == NULL) { continue; }

      (*ilevel)->Clean(ed);
      extraCritters.push_back(*ilevel);
    }
    level.clear();
  }

  return;
}

CreatureInfo *hfcPopulationStorageBase::GenerateRandom(
				       RandomNumberGenerator &rng,
				       LanguageEngine &le,
				       EnvironmentData &ed,
				       const vector<CreatureInfo *> &critters,
				       const vector<double> &fitnessSpread)
{
  double maxFit = fitnessSpread.back();
  double chance = rng.GetDouble() * maxFit;

  unsigned int j;
  for(j = 0; j < critters.size(); j++){
    if(chance < fitnessSpread.at(j)) { break; }
  }

  CreatureInfo *oci = critters.at(j);
  if(oci == NULL) { return NULL; }

  CreatureInfo *nci = NULL;
  if(!extraCritters.empty()) {
    nci = extraCritters.back();
    extraCritters.pop_back();
  } else{
    // Should never occur
    cerr << "This should never occur: new creature info" << endl;
    nci = new CreatureInfo();
  }

  Spider &spider = le.GetSpider();
  Turtle &turtle = le.GetTurtle();
  if(!oci->GetCreature().AsexualReproduction(nci->GetCreature(), ed,
					     spider, turtle)) {
    return NULL;
  }

  nci->Fitness() = oci->Fitness();
  nci->ParentId() = oci->Id();
  nci->Pause() = false;
  nci->Still() = false;
  nci->Id() = d_currentId;
  d_currentId++;

  return nci;
}

unsigned int hfcPopulationStorageBase::getSubPop(CreatureInfo &ci) const
{
  if(ADTs.empty()) { return 0; }

  // Assuming that the fitnesses for each level is in order from low to high
  for(unsigned int i = 0; i < ADTs.size() - 1; i++)
  {
    double fitness = ci.Fitness();

    if(fitness < ADTs.at(i+1))
    {
      return i;
    }
  }

  return ADTs.size() - 1;
}
