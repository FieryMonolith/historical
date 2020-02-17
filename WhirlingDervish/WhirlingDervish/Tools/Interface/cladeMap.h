#ifndef _CLADE_MAP_H_
#define _CLADE_MAP_H_

#include <vector>

#include "creature_info.h"
//#include "environment_data.h"

using namespace std;

class EnvironmentData;


class cladeData{

 public:
     cladeData():
          Next(NULL),
          clade(0) {}
     
     cladeData(unsigned num):
          Next(NULL),
          clade(num) {}

     ~cladeData(){}

     unsigned getClade() const {return clade;}
     const vector<CreatureInfo*> & getCritterIDs() const {return critterIDs;}

     unsigned size() const {return critterIDs.size();}
     void push_back(CreatureInfo* id) {critterIDs.push_back(id); return;} 
     CreatureInfo* at(unsigned id) const {return critterIDs.at(id);}
     void erase(CreatureInfo* id);

     unsigned cladeSize() {return critterIDs.size();}

     cladeData *getNext() { return Next;}
     void setNext(cladeData* n) {Next = n;}

     void clean(EnvironmentData& ed, vector<CreatureInfo*>& extraCritters);


 private:
     cladeData *Next;

     unsigned clade;
     vector<CreatureInfo*> critterIDs;

};


class cladeMap{

 public:
     cladeMap():
          mapSize(100)
     {
          map.reserve(mapSize);
          for(unsigned i = 0; i < mapSize; i++) map.push_back(NULL);
     }

     cladeMap(unsigned size):
          mapSize(size)
     {
          map.reserve(mapSize);
          for(unsigned i = 0; i < mapSize; i++) map.push_back(NULL);
     }

     ~cladeMap() {} 

     cladeData* find(unsigned id);

     unsigned numClades() {return heap.size();}
     unsigned size();

     cladeData* at(unsigned i) {return heap.at(i);}

     void insert(cladeData* cd);
     void remove(unsigned clade);
     void remove(CreatureInfo* id);
     void clean(EnvironmentData& ed, vector<CreatureInfo *>& extraCritters);
     
 private:
     
     unsigned hash(unsigned id){ return id % mapSize;}

     void swap(cladeData* first, cladeData* second);

     unsigned mapSize;

     vector<cladeData*> map;
     vector<cladeData*> heap;
};

#endif
