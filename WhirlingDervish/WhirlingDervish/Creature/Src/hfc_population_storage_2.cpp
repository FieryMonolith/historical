/***********************************
   Art Covert -- 9/2004
   HFC population storage class for WhirlingDervish
 ***********************************/

//#define debug

#define DBL_MIN   2.2250738585072014e-308

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iomanip>

#include "random.h"
#include "environment_data.h"
#include "creature_info.h"
#include "language_engine.h"
#include "spider.h"
#include "turtle.h"

#include "hfc_population_storage_2.h"

using namespace std;

hfcPopulationStorage2::hfcPopulationStorage2(void)
  : d_bufferMax(500)
{
  return;
}

hfcPopulationStorage2::~hfcPopulationStorage2(void) {
  return;
}

bool hfcPopulationStorage2::Initialize(unsigned int size,
				       EnvironmentData &ed,
				       int language) {
  if(!hfcPopulationStorageBase::Initialize(size, ed, language)) {
    return false;
  }

  out.open("suppopulation_fitness.txt",ios::out);
  if(!out) { return false; }

  d_subpopsize.push_back(d_populationSize);
  d_subpopsize.push_back(0);
  d_subpopsize.push_back(0);
  d_subpopsize.push_back(0);

  d_subpopCoef.push_back(1.0);
  d_subpopCoef.push_back(1.0);
  d_subpopCoef.push_back(1.0);
  d_subpopCoef.push_back(1.0);

  d_effectiveSubPopSize.push_back(300);
  d_effectiveSubPopSize.push_back(0);
  d_effectiveSubPopSize.push_back(0);
  d_effectiveSubPopSize.push_back(0);

  for(unsigned int i=0; i<d_bufferMax; i++) {
    CreatureInfo *ci = new CreatureInfo();
    if(ci == NULL) {
      Clear(ed);
      return false;
    }
    ci->Clean(ed);
    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Language() = language;
    ci->Id() = d_currentId;
    d_currentId++;

    d_bufferExcess.push_back(ci);
  }

  return true;
}

void hfcPopulationStorage2::Clear(EnvironmentData &ed) {
  hfcPopulationStorageBase::Clear(ed);

  vector<CreatureInfo *>::iterator i;
  for(i=d_buffer.begin(); i!=d_buffer.end(); ++i) {
    d_bufferExcess.push_back(*i);
  }
  d_buffer.clear();

  for(i=d_bufferExcess.begin(); i!=d_bufferExcess.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }
  d_bufferExcess.clear();

  return;
}

/*********************
 Perform reprodcution for a generation
 *********************/
bool hfcPopulationStorage2::Reproduce(EnvironmentData &ed,
				     RandomNumberGenerator &rng,
				     LanguageEngine &le)
{
#if 1
  cerr << "Reproduce_top" << endl;
#endif

  d_Generation++;

  vector<double> fitnessSpread;
  double maxFit = 0.0;
  double chance = 0;
  unsigned size = 1;
  CreatureInfo *oci, *nci;

  for(int i = 0; i < 4; i++) d_a.at(i).clear();

  /*
    shuffle the children so that they have an equal chance of being placed in
    a subpopulation as opposed to the buffer.
  */
  
  if(d_Generation < d_startHFC){
    //do not use hfc
    //put creatures in first level only
#if 1
    cerr << "Reproduce_branch 1" << endl;
#endif

    vector<CreatureInfo*>& critters = d_a.at(0);
    for(unsigned i = 0; i < allCreatures.size(); i++){
      if(allCreatures.at(i) != NULL){
	critters.push_back(allCreatures.at(i));
      }
    }

  }
  else if((d_Generation % 5) == 0){
#if 1
    cerr << "Reproduce_branch 2" << endl;
#endif

    shuffleAllCreatures(rng);

    if(d_Generation == 20){
      for(int i = 0; i < d_subpopsize.size(); i++){
	d_subpopsize.at(i) = d_populationSize;
	d_effectiveSubPopSize.at(i) = static_cast<int>(d_subpopsize.at(i) * d_subpopCoef.at(i));
      }
    }
   //use hfc recompute adaptive thresholds  
    getFitnessSpread(allCreatures, fitnessSpread);

    //find the fitnessSpread
    //getFitnessSpread(allCreatures, fitnessSpread);


    //compute ADT
    computeADT(fitnessSpread);
 
    //divide population into HFC subpopulations
    divideAllCreatures(ed);

 }else{
#if 1
    cerr << "Reproduce_branch 3" << endl;
#endif
    shuffleAllCreatures(rng);
   //use hfc do not recompute thresholds

    //divide population into HFC subpopulations
    divideAllCreatures(ed);
  }

  allCreatures.clear();

  cerr << "Cleared creaturs: " << allCreatures.size() << endl;
  
  for(int i = 0; i < 4; i++){
    vector<CreatureInfo*>& critters = d_a.at(i);

    if(critters.size() > 0) {
      size = 1;
#if 0
      cerr << "Reporduce_here start" << endl;
#endif
      cerr << "Reproducing population " << i << endl;
      cerr << "subpopulation has : " << critters.size() << " creatures." << endl;
      
      getNormalizedFitness(critters, fitnessSpread);

      maxFit = fitnessSpread.back();

      while(size <= d_effectiveSubPopSize.at(i)){

	chance = rng.GetDouble() * maxFit;

	unsigned j;
	for(j = 0; j < critters.size(); j++){
	  if(chance < fitnessSpread.at(j)) break;
	}
      
	oci = critters.at(j);
	if(extraCritters.size() > 0){
	  nci = extraCritters.at(extraCritters.size() -1);
	  extraCritters.pop_back();
	}
	else{
	  nci = new CreatureInfo();
	  critterCount++;
	}

	Spider &spider = le.GetSpider(oci->Language());
	Turtle &turtle = le.GetTurtle(oci->Language());

	if(!oci->GetCreature().AsexualReproduction(nci->GetCreature(), ed,
						   spider, turtle)) {
	  Clear(ed);
	  return false;
	} 
#if 0
	cerr << "Reporduce_here end" << endl;
#endif 
	nci->Fitness() = oci->Fitness();
	nci->ParentId() = oci->Id();
	nci->Language() = oci->Language();
	nci->Pause() = false;
	nci->Still() = false;
	nci->Id() = d_currentId;
	d_currentId++;
	size++;
      
	allCreatures.push_back(nci);

      }

    }
  }

  clearSubpopulation(ed);
#if 1
  cerr << "Reproduce_bottom " << endl;
#endif
  return true;
}

/**************************
compute the Adaptive Threshholds for the next population
**************************/
void hfcPopulationStorage2::computeADT(vector<double>& fitnessSpread)
{
  hfcPopulationStorageBase::computeADT(fitnessSpread);

  //from Hu et al, 
  ADTs.at(0) = DBL_MIN;
  ADTs.at(1) = fAvg;
  ADTs.at(3) = fMax - fStdDev;
  ADTs.at(2) = ((ADTs.at(3) - fAvg)/2) + fAvg;

#if 1
  cout << "Addaptive Thresholds set:" << endl;
  cout << showpoint << fixed << setprecision(6);
  cout << setw(10) << "Level 0: " << ADTs.at(0) << endl; 
  cout << setw(10) << "Level 1: " << ADTs.at(1) << endl; 
  cout << setw(10) << "Level 2: " << ADTs.at(2) << endl; 
  cout << setw(10) << "Level 3: " << ADTs.at(3) << endl; 
#endif

  return;
}

void hfcPopulationStorage2::divideAllCreatures(EnvironmentData& ed)
{
  unsigned bsizeStart = d_buffer.size(),
           badd       = 0,
           berase     = 0,
           bsizeEnd   = 0;

  vector<int> pushed_to;

  unsigned size = allCreatures.size();
  CreatureInfo *ci;

  for(unsigned i = 0; i < size; i++){
    ci = allCreatures.at(i);
    bool placed = false;
    for(int i = d_a.size()-1; i >= 0; i--){
      if((ci->Fitness() > ADTs.at(i))
	 &&(d_a.at(i).size() < d_subpopsize.at(i))){ 
	d_a.at(i).push_back(ci);

	placed = true;
	break;
      }
      else if((ci->Fitness() > ADTs.at(i))
	    &&(d_a.at(i).size() >= d_subpopsize.at(i))){ 
	if(d_buffer.size() > 499){
	  CreatureInfo* oci = *d_buffer.begin();
	  oci->Clean(ed);
	  extraCritters.push_back(oci);
	  
	  d_buffer.erase(d_buffer.begin());
	}

	d_buffer.push_back(ci);
	badd++;

	placed = true;
	break;
      }
    }

    if(!placed){
      if(d_buffer.size() > 499){
	CreatureInfo* oci = *d_buffer.begin();
	oci->Clean(ed);
	extraCritters.push_back(oci);

	d_buffer.erase(d_buffer.begin());
      }
      d_buffer.push_back(ci);
      badd++;
    }
  }
  vector< CreatureInfo *>::iterator critter;

  //check for subpopulations that are not full, try to fill them with what's in the buffer

  //this bit of code tries to fill up the subpopulations with critters from the buffer.
  //rewrite this so that the buffer will use integers instead of itterators??
  for(int i = 0; i < 4; i ++){
    for(critter = d_buffer.begin();(critter != d_buffer.end())&&(d_a.at(i).size() < d_subpopsize.at(i));){
      if((*critter)->Fitness() >= ADTs.at(i)){
	if((((i+1) < ADTs.size())&&((*critter)->Fitness() < ADTs.at(i+1)))
	   ||((i+1) == ADTs.size())){
	  d_a.at(i).push_back((*critter));
	  pushed_to.push_back(i);
	  d_buffer.erase(critter);
	  berase++;
	}
	else{
	  critter++;
	}
      }
      else critter++;
    }
  }

  bsizeEnd = d_buffer.size();


#if 1
  cout << "Subpopulation sizes" << endl;
  cout << "Level 1: " << d_a.at(0).size() << endl;
  cout << "Level 2: " << d_a.at(1).size() << endl;
  cout << "Level 3: " << d_a.at(2).size() << endl;
  cout << "Level 4: " << d_a.at(3).size() << endl;
  cout << "Buffer : " << d_buffer.size() << endl;
  cout << "Buffera: " << badd << endl;
  cout << "Buffere: " << berase << endl;
  cout << "Bufferd: " << bsizeEnd - bsizeStart << endl;
  cout << "extra:   " << extraCritters.size() << endl;

  out << "Addaptive Thresholds set: ";
  out << showpoint << fixed << setprecision(6);
  out << setw(10) << ADTs.at(0) << " "; 
  out << setw(10) << ADTs.at(1) << " "; 
  out << setw(10) << ADTs.at(2) << " "; 
  out << setw(10) << ADTs.at(3) << endl; 

  vector<double> tempAvg;
  getFitnessSpread(d_a.at(0), tempAvg);
  sort(tempAvg.begin(), tempAvg.end());
  out << d_a.at(0).size() << "(" << average(tempAvg) << ", " << tempAvg.front() << ", " << tempAvg.back() << ") ";
  tempAvg.clear();
  getFitnessSpread(d_a.at(1), tempAvg);
  sort(tempAvg.begin(), tempAvg.end());
  out << d_a.at(1).size() << "(" << average(tempAvg) << ", " << tempAvg.front() << ", " << tempAvg.back() << ") ";
  tempAvg.clear();
  getFitnessSpread(d_a.at(2), tempAvg);
  sort(tempAvg.begin(), tempAvg.end());
  out << d_a.at(2).size() << "(" << average(tempAvg) << ", " << tempAvg.front() << ", " << tempAvg.back() << ") ";
  tempAvg.clear();
  getFitnessSpread(d_a.at(3), tempAvg);
  sort(tempAvg.begin(), tempAvg.end());
  out << d_a.at(3).size() << "(" << average(tempAvg) << ", " << tempAvg.front() << ", " << tempAvg.back() << ") ";
  out << endl;

  //out.close();
#endif

}

void hfcPopulationStorage2::shuffleAllCreatures(RandomNumberGenerator &rng)
{
  vector<CreatureInfo *>::iterator itr;
  vector<CreatureInfo *> temp;
  //cerr << "start_shuffle" << endl;
  int i = allCreatures.size() - 1;
  while( i > 0 ){
    itr = allCreatures.begin();
    int offset = rng.GetUInt(i);
    itr += offset;

    temp.push_back(*itr);
    allCreatures.erase(itr);
    i = allCreatures.size();
    //cerr << "in_shuffle" << endl;
  }

  temp.push_back(*(allCreatures.begin()));
  allCreatures.clear();
  allCreatures = temp;

  //cerr << "stop_shuffle" << endl;
  return;
}
