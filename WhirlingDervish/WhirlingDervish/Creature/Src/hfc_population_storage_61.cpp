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

#include "hfc_population_storage_6.h"


using namespace std;

hfcPopulationStorage6::hfcPopulationStorage6(void)
{
  return;
}

hfcPopulationStorage6::~hfcPopulationStorage6(void){
     return;
}

bool hfcPopulationStorage6::Initialize(unsigned int size,
                                          EnvironmentData &ed)
{
  //cladeOut.open("reproduction_density.txt", ios::out);
  replicateOut.open("replication_data.txt", ios::out);
  Clear(ed);
  d_populationSize = size;
  if ( size <= 0) return true;

  unsigned int numOfCritters = d_populationSize * 3;

  allCreatures.reserve(d_populationSize);
  extraCritters.reserve(numOfCritters);

  CreatureInfo *ci;
  //cerr << "INTIALIZED" << endl;

  for(unsigned int i = 0; i < numOfCritters; i++){
    ci = new CreatureInfo();
    if(ci == NULL){
      Clear(ed);
      cerr << "Error: " << numOfCritters << endl;
      return false;
    }

    ci->Clean(ed);
    ci->ParentId() = 0;
    ci->Fitness() = 0.0;
    ci->Id() = d_currentId;
    //ci->Clade() = d_clade;
    //ci->parentClade() = 0;
    d_currentId++;
    //d_clade++;

    extraCritters.push_back(ci);
  }

  for(unsigned int i = 0; i <d_populationSize; i++){
    ci = extraCritters.back();
    extraCritters.pop_back();

    allCreatures.push_back(ci);
  }

  //InitializeLocal(size, ed, language);
  //cerr << "INTIALIZED_END " << allCreatures.size()<< endl;
  return true;
}

bool hfcPopulationStorage6::InitializeLocal(unsigned int size, EnvironmentData & ed){
     d_Generation = 0;
     CreatureInfo* ci;


     for(unsigned i = 0; i < d_populationSize; i++){
          ci = new CreatureInfo();
          if(ci == NULL){
               Clear(ed);
               return false;
          }

          ci->Clean(ed);
          ci->ParentId() = 0;
          ci->Fitness() = 0.0;
          ci->Id() = d_currentId;
          ci->parentClade() = 0;
          d_currentId++;
          d_clade++;

          allCreatures.push_back(ci);
          
     }

     return true;
}

void hfcPopulationStorage6::Clear(EnvironmentData& ed){
  /*
     ClearLocal(ed);
     d_populationSize = 0;

     vector<CreatureInfo*>::iterator i;
     
     for(i=allCreatures.begin(); i!=allCreatures.end(); ++i) {
          extraCritters.push_back(*i);
     }
     allCreatures.clear();

     for(unsigned i = 0; i < extraCritters.size(); i++){
          CreatureInfo* ci = extraCritters.at(i);
          if(ci != NULL){
               ci->Clean(ed);
          }
     }

     extraCritters.clear();
  */

}

bool hfcPopulationStorage6::Reproduce(EnvironmentData &ed,
                                      RandomNumberGenerator &rng,
                                      LanguageEngine &le){

  //cerr << "Reproduce::Top" << endl;
  d_Generation++;
  replicateOut << "GENERATION: " << ed.Generation() << endl;
  PreprocessAllCreatures(ed);

  replicateOut << "Max: " << d_fMax << endl
	       << "Min: " << d_fMin << endl;
  d_traitSelection.clear();
  for(unsigned i = 0; i < 5; i++) d_traitSelection.push_back(0);

  unsigned targetFit = 0;
  //double tau = 0.0;
  vector<unsigned> fits;
  for(unsigned i = 0; i <= d_fMax; i++){fits.push_back(0);}


  while(allCreatures.size() < d_populationSize){
    //tau = rng.GetDouble(1.0);
    if (d_fMax == 0) targetFit = 0;
    else if ((allCreatures.size() < d_populationSize * (2.0/3.0)) 
	     && (d_fMax > 2)){
      replicateOut << "Branch 2" << endl;
      targetFit = rng.GetUInt(d_fMax - (d_fMin - 2) + 1) + (d_fMax - 2);
    }
    else {
      replicateOut << "Branch 3" << endl;
      targetFit = rng.GetUInt(d_fMax + 1);
    }

    replicateOut << "TargetFit: " << targetFit << endl;

    
    //from CHFC Hu, et al
    /*
    if((d_fMax > 5) && (allCreatures.size() < (d_populationSize / 3)))
      targetFit = (d_fMax - 3) + tau * ((d_fMax + 1) - d_fMin);
    else
      targetFit = d_fMin + tau * ((d_fMax + 1) - d_fMin);
    */

    unsigned targetTrait = rng.GetUInt(5);
    
    vector<CreatureInfo*> candiates;
    getCandidates(targetTrait, targetFit, candiates);

    replicateOut << "Potentials: " << candiates.size() << endl;

    if(candiates.size() > 0){
      fits.at(targetFit)++;
      unsigned index = rng.GetUInt(candiates.size());
      
      CreatureInfo* oci;
      oci = candiates.at(index);
      if(oci == NULL) return false;
      CreatureInfo* nci = NULL;
      
      candiates.clear();
      
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
	return false;
      }
      
      nci->Fitness() = oci->Fitness();
      nci->parentFitness() = oci->Fitness();
      nci->ParentId() = oci->Id();
      replicateOut << "parentFitness: " << oci->Id() << endl;
      nci->Pause() = false;
      nci->Still() = false;
      nci->Id() = d_currentId;
      nci->Clade() = oci->Clade();
      nci->parentClade() = oci->Clade();
      d_currentId++;
      
      //add new organism to new cladeMap and allCreatures.
      allCreatures.push_back(nci);
    }
    
  }

  for(unsigned i = 0; i <= d_fMax; i++){
    replicateOut << "Level " << setw(2) << i << " : " << setw(3) << fits.at(i) << endl;
  }

  cout << "We have " << allCreatures.size() << " baby critters."  << endl;
  cout << "We have " << extraCritters.size() << " spare critters." << endl;
  
  for(unsigned i = 0; i < d_traitSelection.size(); i++){
    cerr << i << ": " << d_traitSelection.at(i) << endl;
  }

  for(unsigned i = 0; i < d_newCritters.size(); i++){
    CreatureInfo *ci = d_newCritters.at(i);
    if(ci != NULL){
      ci->Clean(ed);
      extraCritters.push_back(ci);
    }
  }
  d_newCritters.clear();

  return true;

}

void hfcPopulationStorage6::ClearLocal(EnvironmentData& ed){
    
     allCreatures.clear();

     for(unsigned i = 0; i < extraCritters.size(); i++){
          if(extraCritters.at(i) != NULL){
               extraCritters.at(i)->Clean(ed);
          }

          delete extraCritters.at(i);
     }

     return;
    
}

void hfcPopulationStorage6::InitializeADT(void){
     return;
}

void hfcPopulationStorage6::DetermineADTValues( vector<double> &fitnesssSpread,
                         double fMax, double fAvg, double fStdDev){
     return;
}

void hfcPopulationStorage6::PreprocessAllCreatures(EnvironmentData &ed){
  d_fMin = 10000;
  d_fMax = 0;
  vector<unsigned> levels;

  for(unsigned j = 0; j < d_populationSize; j++){
    CreatureInfo* ci = allCreatures.at(j);
  
    if(ci->parentFitness() <= ci->Fitness()){
      d_newCritters.push_back(ci);
      if(!isin(levels, (unsigned)ci->Fitness())){
	levels.push_back((unsigned)ci->Fitness());
      }
      
      if(ci->Fitness() > d_fMax) d_fMax = (unsigned)ci->Fitness();
      if(ci->Fitness() < d_fMin) d_fMin = (unsigned)ci->Fitness();
      
    }
    else{
      if(ci != NULL){
	ci->Clean(ed);
	extraCritters.push_back(ci);
      }
    }
  }

  allCreatures.clear();
}

bool hfcPopulationStorage6::isin(vector<unsigned> &levels, unsigned search){
  for(unsigned i = 0; i < levels.size(); i++){
    if(levels.at(i) == search) return true;
  }
  return false;
}

void hfcPopulationStorage6::HandleOverflow( vector<unsigned int> & idsNotAdded,
                                            EnvironmentData &ed){
     return;
}

void hfcPopulationStorage6::cleanSubpopulations(EnvironmentData& ed){
     //clean the subpopulation
     return;
}

void hfcPopulationStorage6::clearSubpopulations(EnvironmentData&){
     //erase everything
}

CreatureInfo* hfcPopulationStorage6::GenerateRandom(RandomNumberGenerator &, LanguageEngine &,
                                                    EnvironmentData &,
                                                    const std::vector<CreatureInfo *> &critters,
                                                    const std::vector<double> &fitnessSpread){
     return NULL;
}


unsigned int hfcPopulationStorage6::getSubPop(CreatureInfo * ci) const{
     return static_cast<unsigned>(ci->Fitness());
}

void hfcPopulationStorage6::getCandidates(unsigned targetTrait, unsigned targetFit, vector<CreatureInfo*> &candidates){
  unsigned upperBound = targetFit;
  unsigned count = 0;
  unsigned traitMax = 0;

  //if((static_cast<unsigned>(targetFit * 100) % 100) < 50) 
  //upperBound = (unsigned)targetFit;


  d_traitSelection.at(targetTrait)++;
  
  vector<CreatureInfo*> potentials;

  for(unsigned i = 0; i < d_newCritters.size();i++){
    CreatureInfo *ci = d_newCritters.at(i);
    
    if(ci->Fitness() == upperBound /*|| ci->Fitness() == lowerBound*/){
      count++;
      potentials.push_back(ci);
      if(ci->vFitness().at(targetTrait) > traitMax) traitMax = ci->vFitness().at(targetTrait);
    }
  }

  for(unsigned i = 0; i < potentials.size(); i++){
    CreatureInfo *ci = potentials.at(i);
    
    if(ci->vFitness().at(targetTrait) == traitMax) candidates.push_back(ci);
  }

  return;
}

const vector<CreatureInfo*>& hfcPopulationStorage6::Population(void) const{
     return allCreatures;
}

vector<CreatureInfo*>& hfcPopulationStorage6::Population(void){
     return allCreatures;
}


