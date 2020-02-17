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

#include "hfc_population_storage_5.h"


using namespace std;

hfcPopulationStorage5::hfcPopulationStorage5(void):
     d_numberOfLevels(50),
     d_clade(1)
{
     return;
}

hfcPopulationStorage5::~hfcPopulationStorage5(void){
     return;
}

bool hfcPopulationStorage5::Initialize(unsigned int size,
                                          EnvironmentData &ed)
{
     cladeOut.open("reproduction_density.txt", ios::out);
     replicateOut.open("replication_data.txt", ios::out);
     Clear(ed);
     d_populationSize = size;
     if ( size <= 0) return true;

     vector<CreatureInfo*> temp;
     temp.reserve(d_populationSize);

     d_buffer.reserve(d_numberOfLevels);

     for(unsigned int i = 0; i<d_numberOfLevels; i++){
          d_buffer.push_back(temp);
     }

     unsigned int numOfCritters = d_populationSize * d_numberOfLevels * 3;

     allCreatures.reserve(d_populationSize * d_numberOfLevels);
     extraCritters.reserve(numOfCritters);

     CreatureInfo *ci;

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
          //ci->Language() = language;
          ci->Id() = d_currentId;
          ci->Clade() = d_clade;
          ci->parentClade() = 0;
          d_currentId++;
          d_clade++;

          extraCritters.push_back(ci);
     }

     for(unsigned int i = 0; i <d_populationSize; i++){
          ci = extraCritters.back();
          extraCritters.pop_back();

          //d_buffer.at(0).push_back(ci);
          allCreatures.push_back(ci);
     }

     for(unsigned int i = 0; i < d_numberOfLevels; i++){
          cladeMap *cd = new cladeMap(d_populationSize);
          d_cladeLevels.push_back(cd);
     }


     //InitializeLocal(size, ed, language);

     return true;
}

bool hfcPopulationStorage5::InitializeLocal(unsigned int size, EnvironmentData & ed){
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
          //ci->Language() = language;
          ci->Id() = d_currentId;
          ci->Clade() = d_clade;
          ci->parentClade() = 0;
          d_currentId++;
          d_clade++;

          allCreatures.push_back(ci);
          placeInMap(ci);
          
     }

     return true;
}

void hfcPopulationStorage5::Clear(EnvironmentData& ed){
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

bool hfcPopulationStorage5::Reproduce(EnvironmentData &ed,
                                      RandomNumberGenerator &rng,
                                      LanguageEngine &le){
     //cerr << "Reproduce::Top" << endl;
     d_Generation++;
     
     cladeOut << "Generation: " << d_Generation << endl;
     PreprocessAllCreatures(ed);


     for(unsigned int i=0;i<d_numberOfLevels;i++){
          //cerr << "Loop 1: Start " << i << endl;
          cladeMap* currClade = d_cladeLevels.at(i);
          
          if(currClade->size() == 0) break;

          unsigned n = currClade->numClades();
          unsigned weakCap = d_populationSize /n;
          unsigned hardCap = (d_populationSize / n) * 2;

          unsigned tickets = 0;

          /*cerr << "weakCap : " << weakCap
            << "hardCap : " << hardCap << endl;*/

          vector<unsigned> reproductionDensity;
          replicateOut << "Level: " << i << " " << currClade->size() << " ";
          unsigned lonely = 0;
          cladeOut << "Level: " << i << endl;
          if(n > 1 && currClade->size() > 60)
               replicateOut << 1 << " ";
          else
               replicateOut << 0 << " ";
          for(unsigned j = 0; j < n; j++){
               if(currClade->at(j)->cladeSize() > hardCap) {
                    tickets = 1;
                    cladeOut << setw(5) << currClade->at(j)->getClade() << ":" << currClade->at(j)->cladeSize() << ":" << 1 << endl;
               }
               if(currClade->at(j)->cladeSize() > weakCap &&
                       currClade->at(j)->cladeSize() <= hardCap) {
                    tickets = 2;
                    cladeOut << setw(7) << currClade->at(j)->getClade() << ":" << currClade->at(j)->cladeSize() << ":" << 2 << endl;
               }
               if(currClade->at(j)->cladeSize() <= weakCap){
                    tickets = 3;
                    cladeOut << setw(7) << currClade->at(j)->getClade() << ":" << currClade->at(j)->cladeSize() << ":" << 3 << endl;
               }
               if(currClade->at(j)->cladeSize() == 1) lonely++;
               for(unsigned N = 0; N < tickets * currClade->at(j)->size(); N++)
                                        reproductionDensity.push_back(currClade->at(j)->getClade());
               tickets = 0;
          }
          replicateOut << lonely << endl;
          cladeOut << "Level " << i << " size: " << currClade->numClades() << endl;
          cladeOut << "--------------------------" << endl;
          /*cladeOut << reproductionDensity.size() << ": ";
          for(unsigned ii = 0; ii < reproductionDensity.size(); ii++){
               cladeOut << reproductionDensity.at(ii) << ", ";
               }*/

          cladeOut << endl;


          //do this for the population size
          for(unsigned j = 0; j < d_populationSize; j++){
               //cerr << "Loop 2: Start " << i << endl;
               //randomly select clade
               unsigned num = rng.GetUInt(reproductionDensity.size() - 1);
               /*cerr << "Loop 2: Here " << num 
                    << " " << reproductionDensity.size() 
                    << " " << currClade->size()<< endl;*/
               //from that clade randomly select organism to reproduce
               cladeData* cd = currClade->find(reproductionDensity.at(num));
          
               num = rng.GetUInt(cd->size()-1);
               CreatureInfo* oci;

               if(cd->size() != 0){
                    oci = cd->at(num);
               }else{
                    oci = cd->at(0);
               }

               if(oci == NULL) return false;
               CreatureInfo* nci = NULL;

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
               nci->ParentId() = oci->Id();
               //nci->Language() = oci->Language();
               nci->Pause() = false;
               nci->Still() = false;
               nci->Id() = d_currentId;
               nci->Clade() = oci->Clade();
               nci->parentClade() = oci->Clade();
               d_currentId++;
               

               replicateOut << "Replicating " << cd->getClade() 
                            << " of size " << cd->size()
                            << " at " << num 
                            << " with clade " << oci->Clade() 
                            << " into " << nci->Clade() << endl;

               //add new organism to new cladeMap and allCreatures.
               allCreatures.push_back(nci);
          }
          currClade->clean(ed, extraCritters);
     }
     cladeOut << "*************************" << endl;
     cout << "We have " << allCreatures.size() << " baby critters."  << endl;
     cout << "We have " << extraCritters.size() << " spare critters." << endl;
     return true;

}

void hfcPopulationStorage5::ClearLocal(EnvironmentData& ed){
     for(unsigned i = 0; i < d_cladeLevels.size(); i++){ 
          d_cladeLevels.at(i)->clean(ed, extraCritters);
     }

     allCreatures.clear();

     for(unsigned i = 0; i < extraCritters.size(); i++){
          if(extraCritters.at(i) != NULL){
               extraCritters.at(i)->Clean(ed);
          }

          delete extraCritters.at(i);
     }

     return;
    
}

void hfcPopulationStorage5::InitializeADT(void){
     return;
}

void hfcPopulationStorage5::DetermineADTValues( vector<double> &fitnesssSpread,
                         double fMax, double fAvg, double fStdDev){
     return;
}

void hfcPopulationStorage5::PreprocessAllCreatures(EnvironmentData &ed){
     unsigned critterCount = 0;
     for(unsigned i = 0; i < d_numberOfLevels; i++){
          //cladeOut << "Level " << i << ": "; 
          for(unsigned j = 0; j < d_populationSize && critterCount < allCreatures.size(); j++){
               unsigned currLevel;

               currLevel = i;

               CreatureInfo* ci = allCreatures.at(critterCount);
               critterCount++;

               //cladeOut << ci->Clade() << " ";

               if(ci != NULL){
                    if(currLevel == ci->Fitness()){
                         placeInMap(ci);
                    }
                    else if(currLevel < ci->Fitness()){
                         placeInBuffer(ci);
                    }
                    else if(currLevel > ci->Fitness()){
                         if(ci != NULL){
                              ci->Clean(ed);
                              extraCritters.push_back(ci);
                         }
                    }
               }
          }
          //cladeOut << endl;
     }

     cout << "critterCount :" << critterCount << endl;
     
     for(unsigned i = 0; i < d_cladeLevels.size(); i++){
          while(d_cladeLevels.at(i)->size() < d_populationSize&&
               d_buffer.at(i).size() > 0){
               cladeData *cd = new cladeData((*d_buffer.at(i).begin())->Clade());
               cd->push_back(d_buffer.at(i).at(0));
               d_buffer.at(i).erase(d_buffer.at(i).begin());

               d_cladeLevels.at(i)->insert(cd);
          }
     }

     allCreatures.clear();
}

void hfcPopulationStorage5::HandleOverflow( vector<unsigned int> & idsNotAdded,
                                            EnvironmentData &ed){
     return;
}

void hfcPopulationStorage5::cleanSubpopulations(EnvironmentData& ed){
     //clean the subpopulations -- leave the cladeMaps intact

     for(unsigned i = 0; i < d_numberOfLevels; i++){
          d_cladeLevels.at(i)->clean(ed, extraCritters);
     }

     return;
}

void hfcPopulationStorage5::clearSubpopulations(EnvironmentData&){
     //erase everything

     for(unsigned i = 0; i < d_numberOfLevels; i++){
     }
}

CreatureInfo* hfcPopulationStorage5::GenerateRandom(RandomNumberGenerator &, LanguageEngine &,
                                                    EnvironmentData &,
                                                    const std::vector<CreatureInfo *> &critters,
                                                    const std::vector<double> &fitnessSpread){
     return NULL;
}


unsigned int hfcPopulationStorage5::getSubPop(CreatureInfo * ci) const{
     return static_cast<unsigned>(ci->Fitness());
}

void hfcPopulationStorage5::placeInMap(CreatureInfo* ci){
     unsigned level = static_cast<unsigned>(ci->Fitness());
     cladeMap* cm;


     if(level <= d_numberOfLevels) 
          cm = d_cladeLevels.at(level);
     else{
          //cerr << "Level out of range***********" << level << endl;
          return;
     }
     
     if(cm != NULL){
          //cerr << "Placing in clade: " << ci->Clade() << endl;
          cladeData* cd = cm->find(ci->Clade());

          if(cd != NULL){
               cd->push_back(ci);
          }else{
               cd = new cladeData(ci->Clade());
               cd->push_back(ci);
               cm->insert(cd);
          }
          return;
     }else{
          //cerr << "Cannot find cladeMap************* " << level << endl;
     }


     return;

}

void hfcPopulationStorage5::placeInBuffer(CreatureInfo* ci){
     unsigned level = static_cast<unsigned>(ci->Fitness());

     cladeMap* cm;

     if(level <= d_numberOfLevels) 
          cm = d_cladeLevels.at(level);
     else{
          //cerr << "Level out of range***********" << level << endl;
          return;
     }

     vector<CreatureInfo*>& buffer = d_buffer.at(level);

     if(buffer.size() > 100) {
          buffer.erase(buffer.begin());
     }
     
     ci->Clade() = d_clade++;

     buffer.push_back(ci);

     return;
}

const vector<CreatureInfo*>& hfcPopulationStorage5::Population(void) const{
     return allCreatures;
}

vector<CreatureInfo*>& hfcPopulationStorage5::Population(void){
     return allCreatures;
}


