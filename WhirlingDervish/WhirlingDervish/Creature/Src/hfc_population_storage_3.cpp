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
//#include <fstream>
#include <sstream>
#include <iomanip>

#include "random.h"
#include "environment_data.h"
#include "creature_info.h"
#include "language_engine.h"
#include "spider.h"
#include "turtle.h"

#include "hfc_population_storage_3.h"

using namespace std;

hfcPopulationStorage::hfcPopulationStorage(void):d_Generation(0),d_currentID(0),critterCount(0)
{
#ifdef debug
  cerr << "constructor" << endl;
#endif
  return;
}

hfcPopulationStorage::~hfcPopulationStorage(void) {
  return;
}


vector<CreatureInfo*>& hfcPopulationStorage::Population(){
  return allCreatures;
}

const vector<CreatureInfo*>& hfcPopulationStorage::Population() const{
  return allCreatures;
}

#if 1

bool hfcPopulationStorage::Initialize(unsigned size,
				      EnvironmentData &ed,
				      int language) {

#ifdef debug
  cerr << "Initalize_top" << endl;
#endif

  // I can get away with this because multiple languages are not supported
  d_lang = language;

  //move this down maybe? maybe not.
  out.open("suppopulation_fitness.txt",ios::out);

  Clear(ed);

  d_populationSize = size;

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

  unsigned populationSize = size; //thanks to jason for changing the name in ipopulation

  if(populationSize <= 0) { return true; }

  //create levels for hfc
  d_a.reserve(4);

  for(int i = 0; i < 4; i++){
    vector<CreatureInfo*> temp;
    temp.reserve(populationSize);
    d_a.push_back(temp);
  }

  for(int i = 0; i < 4; i++){
    vector<CreatureInfo*> temp;
    temp.reserve(populationSize);
    d_b.push_back(temp);
  }

  for(int i = 0; i < 4; i++){
    vector<CreatureInfo*> temp;
    temp.reserve(populationSize);
    d_buffer.push_back(temp);
  }


  allCreatures.reserve(populationSize * 4);

  CreatureInfo *ci;
  for(int i=0; i< d_populationSize; i++) {
    ci = new CreatureInfo();
    critterCount++;
    ci->Clean(ed);
    if(ci == NULL) {
      Clear(ed);
      return false;
    }
    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Language() = language;
    ci->Id() = d_currentId;
    d_currentId++;

    d_a.at(0).push_back(ci);
    allCreatures.push_back(ci);
  }


  for(int i=0; i< d_populationSize; i++) {
    ci = new CreatureInfo();
    critterCount++;
    ci->Clean(ed);
    if(ci == NULL) {
      Clear(ed);
      return false;
    }
    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Language() = language;
    ci->Id() = 0;

    extraCritters.push_back(ci);
  }

  ci = NULL;

#ifdef debug
  cerr << "Initalize_bottom" << endl;
#endif
  return true;
}

bool hfcPopulationStorage::Resize(unsigned int newPopulationSize,
					 EnvironmentData &ed,
					 int language) 
{
#ifdef debug
  cerr << "Resize_top" << endl;
#endif
  CreatureInfo *ci;
  if(newPopulationSize > d_populationSize){
    for(int i = 0; i < newPopulationSize - d_populationSize; i++){
      ci = new CreatureInfo();
      critterCount++;
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Language() = language;
      ci->Id() = d_currentId;
      d_currentId++;

      d_a.at(0).push_back(ci);
    }
  }
  else if (newPopulationSize < d_populationSize){
    getAllCreatures(); // transfere all creatures in hieracrchy to the allCreatures vector
    
    for(int i = 0; i < d_populationSize - newPopulationSize; i++){
      ci = allCreatures.back();
      if(ci != NULL){
	ci->Clean(ed);    
	delete ci;
      }
    }

    divideAllCreatures(ed);
  }

  d_populationSize = newPopulationSize;

  return true;
}

unsigned int hfcPopulationStorage::PopulationSize(void) const {
  return d_populationSize;
}

void hfcPopulationStorage::Clear(EnvironmentData &ed) {
  d_populationSize = 0;
  if(allCreatures.size() == 0) return;
  vector<CreatureInfo *>::iterator i;
  for(i=allCreatures.begin(); i!=allCreatures.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }

  for(i=extraCritters.begin(); i!=extraCritters.end(); ++i) {
    if(*i != NULL) {
      (*i)->Clean(ed);
      delete *i;
      *i = NULL;
    }
  }

  for(unsigned g= 0; g < d_buffer.size(); g++){
    for(i=d_buffer.at(g).begin(); i != d_buffer.at(g).end(); ++i) {
      if(*i != NULL) {
	(*i)->Clean(ed);
	delete *i;
	*i = NULL;
      }
    }
  }

  allCreatures.clear();

  for(int j = 0; j < 4; j++){
    d_a.at(j).clear();
  }
}

/**********************
 Insert a criter to the base population, will be promoted if necessary
 at the next iteration
 **********************/
bool hfcPopulationStorage::Replace(int64 id,int64 &newID, const string &str, 
				  EnvironmentData &ed, LanguageEngine &le,
				  int language) {

#ifdef debug
  cerr << "Replace_top" << endl;
#endif
  CreatureInfo *ci;

  for(unsigned i = 0; i < allCreatures.size(); i++){
    if(allCreatures.at(i)->Id() == id){
      ci = allCreatures.at(i);
      vector<int> errors;
      if(!le.GetParser(language).Parse(ci->GetCreature().GetGenome(),
				       errors, str) || !errors.empty()) 
	{
	  ci->Clean(ed);
	  //d_extra.push_back(ci);
	  return false;
	}
      if(!le.GetSpider(language).Run(ci->GetCreature().GetGenome(),
				     ci->GetCreature().GetBody(),
				     ed)) 
	{
	  ci->Clean(ed);
	  //d_extra.push_back(ci);
	  return false;
	}
      ci->ParentId() = 0;
      ci->Fitness() = 0.0;
      ci->Language() = language;
      ci->Pause() = false;
      ci->Still() = false;
      ci->Id() = d_currentId;
      newID = d_currentId;

      d_currentId++;
      return true;
    }
  }
  return false;
#ifdef debug
  cerr << "Replace_bottom" << endl;
#endif
}

/*********************
 Perform reprodcution for a generation
 *********************/
bool hfcPopulationStorage::Reproduce(EnvironmentData &ed,
				  RandomNumberGenerator &rng,
				  LanguageEngine &le) {

#if 1
  cerr << "Reproduce_top" << endl;
#endif

  d_Generation++;
  vector<double> fitnessSpread;
  //double maxFit = 0.0;
  //double chance = 0;
  //unsigned size = 1;
  //CreatureInfo *oci, *nci;
  
  if(d_Generation < 20){
    //do not use hfc
    //put creatures in first level only
#if 1
    cerr << "Reproduce_branch 1" << endl;
#endif

    vector<CreatureInfo*>& critters = d_b.at(0);
    for(unsigned i = 0; i < allCreatures.size(); i++){
      if(allCreatures.at(i) != NULL){
	critters.push_back(allCreatures.at(i));
      }
    }

  }
  else if((d_Generation % 20) == 0){
#if 1
    cerr << "Reproduce_branch 2" << endl;
#endif

    //shuffleAllCreatures(rng);

    if(d_Generation == 20){
      for(int i = 0; i < d_subpopsize.size(); i++){
	d_subpopsize.at(i) = d_populationSize;
	d_effectiveSubPopSize.at(i) = static_cast<int>(d_subpopsize.at(i) * d_subpopCoef.at(i));
      }
      for(int i = 0; i < allCreatures.size();i++)
	d_a.at(0).push_back(allCreatures.at(i));
    }
   //use hfc recompute adaptive thresholds  
    getFitnessSpread(allCreatures, fitnessSpread);

    //compute ADT
    computeADT(fitnessSpread);
 
    //divide population into HFC subpopulations
    divideAllCreatures(ed);

 }else{
#if 1
    cerr << "Reproduce_branch 3" << endl;
#endif
   //use hfc do not recompute thresholds

    //divide population into HFC subpopulations
    divideAllCreatures(ed);
  }

  cerr << "Out of Reproduction branch" << endl;
  //allCreatures.clear();
  //cerr << "one" << endl;
  //cerr << "two" << endl;
  if(!subpopReproduce(ed, rng, le)) return false;
  //cerr << "three" << endl;
  //clearSubpopulation(ed);
  //clearSubpopulationb(ed);

#if 1
  cerr << "Reproduce_bottom " << endl;
#endif
  return true;
}

/********************
 Remove an organism from the entire population
 ********************/
void hfcPopulationStorage::ReplaceEmpty(int64 id, EnvironmentData &ed) {
  vector<CreatureInfo *>::iterator i;

  for(int j = 0; j < 4; j++)
    for(i = d_a.at(j).begin(); i != d_a.at(j).end(); i++){
      if((*i)->Id() == id){ 
	CreatureInfo *ci;
	(*i)->Clean(ed);
	ci = (*i);
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

bool hfcPopulationStorage::Ids(std::vector<int64> &out) const
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
void hfcPopulationStorage::fitness(vector<double>& fitnessSpread){
  vector<CreatureInfo*>::iterator i;
  double fitness = 0.0;
  for(i = allCreatures.begin(); i != allCreatures.end(); ++i){
    if(*i == NULL) {;}
    else{
      fitness = ((*i)->Fitness() );
      fitnessSpread.push_back(fitness);
      fitness = 0.0;
    }
  }
}

/********************
 get the fitnessSpread for a subuset of the population
 ********************/
void hfcPopulationStorage::getFitnessSpread(vector<CreatureInfo*>& critters, vector<double>& fitnessSpread){
  fitnessSpread.clear();
  for(unsigned i = 0; i < critters.size(); i++){
    fitnessSpread.push_back(critters.at(i)->Fitness());
  }
 
}

/********************
get the normalized fitnesses

same as Jason's for comparison purposes
********************/
void hfcPopulationStorage::getNormalizedFitness(vector<CreatureInfo*>& critters, vector<double>& fitnessSpread){
  fitnessSpread.clear();
 
  sort(critters.begin(), critters.end(), CreatureInfo::Greater);

  double maxFit = 0;

  vector<CreatureInfo*>::iterator i;
  for(i = critters.begin(); i != critters.end(); i++){
    if(*i != NULL){
      fitnessSpread.push_back((*i)->Fitness());
    }
  }

  maxFit = fitnessSpread.at(0);

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
}



/**************************
compute the Adaptive Threshholds for the next population
**************************/
void hfcPopulationStorage::computeADT(vector<double>& fitnessSpread){
#ifdef debug 
  cerr << "fistpread_top" << endl;
#endif
  sort(fitnessSpread.begin(), fitnessSpread.end());



  fMax = fitnessSpread.back();
  fAvg = average(fitnessSpread);
  fStdDev = stdDev(fitnessSpread);

#if 0
  cerr << "fmax    : " << fmax << endl;
  cerr << "fmax    : " << fitnessSpread.back() << endl;
  cerr << "fAvg    : " << fAvg << endl;
  cerr << "fStdDev : " << fStdDev << endl;
#endif
  
  ADTs.clear();
  ADTs.reserve(4);
  for(unsigned i = 0; i < 4; i++) ADTs.push_back(0);

#if 0 
    for(unsigned i = 0; i < 5; i++) cerr << fitnessSpread.at(i) << endl;
    for(unsigned i = fitnessSpread.size() - 5; i < fitnessSpread.size(); i++) cerr << fitnessSpread.at(i) << endl;
#endif

  //from Hu et al, 
  ADTs.at(0) = DBL_MIN;
  ADTs.at(1) = fAvg;
  ADTs.at(3) = fMax - fStdDev;
  ADTs.at(2) = ((ADTs.at(3) - fAvg)/2) + fAvg;
#ifdef debug 
  cerr << "fistpread_here2" << endl;
#endif
#if 1
  cout << "Addaptive Thresholds set:" << endl;
  cout << showpoint << fixed << setprecision(6);
  cout << setw(10) << "Level 0: " << ADTs.at(0) << endl; 
  cout << setw(10) << "Level 1: " << ADTs.at(1) << endl; 
  cout << setw(10) << "Level 2: " << ADTs.at(2) << endl; 
  cout << setw(10) << "Level 3: " << ADTs.at(3) << endl; 
#endif

#ifdef debug 
  cerr << "fistpread_bottom" << endl;
#endif
}

double hfcPopulationStorage::average(vector<double>& fitnessSpread){
  double sum = 0.0;

  for (unsigned i = 0; i < fitnessSpread.size(); i++) sum += fitnessSpread.at(i);

  return sum / fitnessSpread.size();
}

double hfcPopulationStorage::stdDev(vector<double>& fitnessSpread){
  double avg = average(fitnessSpread);
  double sum = 0.0;

  for(unsigned i = 0; i < fitnessSpread.size(); i++) 
    sum += ((fitnessSpread.at(i) - avg)*(fitnessSpread.at(i) - avg));

  sum = sum / fitnessSpread.size();

  return sqrt(sum);
}

void hfcPopulationStorage::getAllCreatures()
{
  allCreatures.clear();
  for(unsigned i = 0; i < d_a.size(); i ++){
    vector<CreatureInfo*>& critters = d_a.at(i);
    for(unsigned j = 0; j < critters.size(); j++)
    {
      if(critters.at(j) != NULL){
	allCreatures.push_back(critters.at(j));
      }
      critters.pop_back();
    }
  }
}

void hfcPopulationStorage::divideAllCreatures(EnvironmentData& ed)
{
  unsigned bsizeStart = d_buffer.size(),
           badd       = 0,
           berase     = 0,
           bsizeEnd   = 0;

  vector<int> pushed_to;

  unsigned size = allCreatures.size();
  CreatureInfo *ci;

  cerr << "startdivide" << endl;
 
  for(unsigned i = d_a.size()-1; i >= 0;i--){
    //d_b.at(i).clear();
    cerr << "subpop inital size: " << d_a.at(i).size() << endl;
    for(unsigned j = 0; j < d_a.at(i).size(); j++){
      ci = d_a.at(i).at(j);
      unsigned pop = getSubPop(ci);
      if(pop < i){
	//remove creature
	if(ci != NULL){
	  ci->Clean(ed);
	  extraCritters.push_back(ci);
	}
	//cerr << "Removing creature " << pop << " " << i << " " << ci->Fitness() << endl;
      }else if(pop >i){
	//promote creature
	cout << "Promoting creature " << pop << " " << i << " " << ci->Fitness() <<  endl;
	cout << "\t" << d_b.at(pop).size() << " " << d_effectiveSubPopSize.at(pop) << endl;
	if(d_b.at(pop).size() >= d_effectiveSubPopSize.at(pop)){
	  d_buffer.at(pop).push_back(ci);
	}
	else{
	  d_b.at(pop).push_back(ci);
	  allCreatures_b.push_back(ci);
	}
	//d_b.at(pop).push_back(ci);
      }
      else if (pop == i){
	//cerr << "Keeping creature " << pop << " " << i << " " << ci->Fitness() << endl;
	if(d_b.at(pop).size() >= d_effectiveSubPopSize.at(pop)){
	  d_buffer.at(pop).push_back(ci);
	}
	else{
	  d_b.at(pop).push_back(ci);
	  allCreatures_b.push_back(ci);
	}
      }
    }
  }

  for(unsigned i = 0; i < d_b.size(); i++){
    cerr << "subpop inital size: " << i << " " << d_b.at(i).size() << endl;
    while((d_b.at(i).size() < d_effectiveSubPopSize.at(i)) && (d_buffer.at(i).size() > 0)){
      if(d_buffer.at(i).back()->Fitness() >= ADTs.at(i)){
	d_b.at(i).push_back(d_buffer.at(i).back());
	d_buffer.at(i).pop_back();
      }
      else{
	d_buffer.at(i).back()->Clean(ed);
	extraCritters.push_back(d_buffer.at(i).back());
	d_buffer.at(i).pop_back();
      }
    }
    cerr << "subpop final size: " << i << " " << d_b.at(i).size() << endl;
  }


  cerr << "enddivide" << endl;


bsizeEnd = d_buffer.size();


#if 1
  cout << "Subpopulation sizes" << endl;
  cout << "Level  1: " << d_b.at(0).size() << endl;
  cout << "Level  2: " << d_b.at(1).size() << endl;
  cout << "Level  3: " << d_b.at(2).size() << endl;
  cout << "Level  4: " << d_b.at(3).size() << endl;
  cout << "Buffer 1: " << d_buffer.at(0).size() << endl;
  cout << "Buffer 2: " << d_buffer.at(1).size() << endl;
  cout << "Buffer 3: " << d_buffer.at(2).size() << endl;
  cout << "Buffer 4: " << d_buffer.at(3).size() << endl;
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

void hfcPopulationStorage::deleteSubpopulation(EnvironmentData &ed){
  vector<CreatureInfo*> &critters = d_a.at(0);
  CreatureInfo* ci;
  unsigned g = 0;

  for(unsigned i = 0; i < d_a.size(); i++){
    g=0;
    for(unsigned j = 0; j < critters.size(); j++){
      ci = d_a.at(i).at(j);
      if(ci != NULL){
	ci->Clean(ed);
	delete ci;
	g++;
      }
    }
    d_a.at(i).clear();
    cerr << "cleared " << g << " creatures from " << i << endl;
    cerr << "popsize : " << d_a.at(i).size() << endl;
  }
}

void hfcPopulationStorage::clearSubpopulation(EnvironmentData &ed){
  vector<CreatureInfo*> &critters = d_a.at(0);
  CreatureInfo* ci;
  unsigned g = 0;

  for(unsigned i = 0; i < d_a.size(); i++){
    g=0;
    for(unsigned j = 0; j < d_a.at(i).size(); j++){
      ci = d_a.at(i).at(j);
      if(ci != NULL){
	/*
	ci->Clean(ed);
	extraCritters.push_back(ci);
	g++;
	*/
      }
    }
    d_a.at(i).clear();
  }
}

void hfcPopulationStorage::clearSubpopulationb(EnvironmentData &ed){
  vector<CreatureInfo*> &critters = d_b.at(0);
  CreatureInfo* ci;
  unsigned g = 0;

  for(unsigned i = 0; i < d_b.size(); i++){
    g=0;
    for(unsigned j = 0; j < d_b.at(i).size(); j++){
      ci = d_b.at(i).at(j);
      if(ci != NULL){
	ci->Clean(ed);
	extraCritters.push_back(ci);
	g++;
      }
    }
    d_b.at(i).clear();
  }
}

void hfcPopulationStorage::shuffleAllCreatures(RandomNumberGenerator &rng){
  vector<CreatureInfo *>::iterator itr;
  vector<CreatureInfo *> temp;

  int i = allCreatures.size() - 1;
  while( i > 0 ){
    itr = allCreatures.begin();
    int offset = rng.GetUInt(i);
    itr += offset;

    temp.push_back(*itr);
    allCreatures.erase(itr);
    i = allCreatures.size();
  }

  temp.push_back(*(allCreatures.begin()));
  allCreatures.clear();
  allCreatures = temp;
  return;
}




bool hfcPopulationStorage::subpopReproduce(EnvironmentData &ed, RandomNumberGenerator &rng, LanguageEngine &le){

  //clearSubpopulation(ed);
  double maxFit = 0,
    //size = 0,
    chance = 0;

  CreatureInfo *nci, *oci;

  clearSubpopulation(ed);
  allCreatures.clear();

  unsigned size;
  for(int i = 0; i < d_b.size();i++){
     cerr << "here1" << endl;
    vector<CreatureInfo *>& critters = d_b.at(i);
    cerr << "here2" << endl;
    if(critters.size() > 0) {
      size = 1;

      vector<double> fitnessSpread;

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

	nci->Fitness() = oci->Fitness();
	nci->ParentId() = oci->Id();
	nci->Language() = oci->Language();
	nci->Pause() = false;
	nci->Still() = false;
	nci->Id() = d_currentId;
	d_currentId++;
	size++;
      
	allCreatures.push_back(nci);
	d_a.at(i).push_back(nci);

      }
      
    }
  }
  clearSubpopulationb(ed);
  allCreatures_b.clear();

  return true;
}

int hfcPopulationStorage::getSubPop(CreatureInfo* ci){
  for(int i = 0; i < ADTs.size(); i++){
    if(i+1 >= ADTs.size()){
      if(ci->Fitness() >= ADTs.at(i))
	return i;
    }
    else{
      if(( ci->Fitness() >= ADTs.at(i) )&&( ci->Fitness() <= ADTs.at(i+1)))
	return i;
    }
  }
  return 0;
}


#endif
