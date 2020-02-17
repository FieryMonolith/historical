#include "cladeMap.h"

using namespace std;


cladeData* cladeMap::find(unsigned id){
     unsigned index = hash(id);

     cladeData* cd = map.at(index);

     while(cd != NULL){
          if(cd->getClade() == id) return cd;
          else cd = cd->getNext();
     } 

     return cd;

}


void cladeMap::insert(cladeData* cd){
     unsigned index = hash(cd->getClade());

     cladeData* curr = map.at(index);

     if(curr == NULL) map.at(index) = cd;
     else{
          while(curr->getNext() != NULL){
               curr = curr->getNext();
          }

          curr->setNext(cd);
     }

     heap.push_back(cd);

     return;

}

void cladeMap::remove(unsigned clade){
     unsigned index = hash(clade);
     cladeData *curr = map.at(index);

     if(curr == NULL) return;

     cladeData *last = NULL;

     while(curr->getClade() != clade){
          last = curr;
          curr = curr->getNext();
     }

     if(last != NULL) last->setNext(curr->getNext());
     delete curr;

     return;
}

void cladeMap::remove(CreatureInfo* id){
     cladeData *cd = find(id->Clade());

     cd->erase(id);

     return;
}


void cladeData::erase(CreatureInfo* id){
     unsigned i = 0;
     for(; i < critterIDs.size(); i++)
          if(critterIDs.at(i) == id){
               critterIDs.erase(critterIDs.begin() + i);
               return;
          }
}

void cladeData::clean(EnvironmentData& ed, vector<CreatureInfo*>& extraCritters){
     for(unsigned i = 0; i < critterIDs.size(); i++){
          CreatureInfo *ci = critterIDs.at(i);
          
          if(ci != NULL){
               ci->Clean(ed);
               extraCritters.push_back(ci);
          }
     }

     if(Next != NULL) Next->clean(ed, extraCritters);

     critterIDs.clear();
     
}

void cladeMap::clean(EnvironmentData& ed, vector<CreatureInfo*>& extraCritters){
     for(unsigned i = 0; i < heap.size(); i++){
          heap.at(i)->clean(ed, extraCritters);
          delete heap.at(i);
     }

     heap.clear();
     map.clear();

     for(unsigned i = 0; i < mapSize; i++){
          map.push_back(NULL);
     }

     return;
}

unsigned cladeMap::size(){
     unsigned s = 0;

     for(unsigned i = 0; i < heap.size(); i++){
          s += heap.at(i)->size();
     }

     return s;
}



