#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "random.h"
#include "ipopulation_storage.h"
#include "language_engine.h"
#include "environment_data.h"
#include "creature_info.h"

#include "file_handler.h"

using namespace std;

FileHandler::FileHandler(void) : d_currentId(1) {
  return;
}

FileHandler::~FileHandler(void) {
  map<int, FileHandle *>::iterator i;
  for(i=d_files.begin(); i!=d_files.end(); ++i) {
    if(i->second == NULL) { continue; }

    delete i->second;
    i->second = NULL;
  }

  return;
}

FileHandler::FileHandle::FileHandle(void) {
  return;
}

FileHandler::FileHandle::~FileHandle(void) {
  if(d_file.is_open()) {
    d_file.close();
  }

  return;
}

int FileHandler::Create(const string &filename, fstream::openmode mode) {
  map<int, FileHandle *>::iterator i = d_files.find(d_currentId);
  // The current id is already in use, return error
  if(i != d_files.end()) { return -2; }

  FileHandle *fh = new FileHandle;
  if(fh == NULL) { return -3; }

  fh->d_id = d_currentId;
  d_currentId++;
  if(d_currentId < 0) { d_currentId = 1; }

  fh->d_filename = filename;
  fh->d_type = mode;

  fh->d_file.open(filename.c_str(), mode);
  if(!fh->d_file) {
    delete fh;
    return -4;
  }

  d_files.insert(make_pair(fh->d_id, fh));

  return fh->d_id;
}

void FileHandler::Close(int fileId) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL) { return; }

  delete i->second;
  i->second = NULL;
  d_files.erase(fileId);

  return;
}

/*
 * BEGIN Save experiment data
 */

/*
 * BEGIN Save experiment Parameters
 */

ostream &FileHandler::OutputStream(int fileId, bool &success) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::out)) {
    success = false;
    return d_defaultStream;
  }

  if(i->second->d_file.rdstate()) {
    success = false;
    return d_defaultStream;
  }

  if(!i->second->d_file.is_open()) {
    success = false;
    return d_defaultStream;
  }

  success = true;
  return i->second->d_file;
}

bool FileHandler::Output(int fileId, int populationSize,
			 int generationQuantity, double timestep) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::out)) {
    return false;
  }

  if(i->second->d_file.rdstate()) {
    return false;
  }

  fstream &out = i->second->d_file;
  if(!out.is_open()) {
    return false;
  }

  // 0 is the language type, which I no longer use
  out << populationSize << " " << generationQuantity << " ";
  out << timestep << " " << 1 << endl;

  return true;
}

/*
 * END Save experiment Parameters
 */

/*
 * BEGIN Save the state for a given generation
 */

bool FileHandler::Output(int fileId, int generation,
			 RandomNumberGenerator &rng, iPopulationStorage &ps) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::out)) {
    return false;
  }

  if(i->second->d_file.rdstate()) {
    return false;
  }

  fstream &out = i->second->d_file;
  if(!out.is_open()) {
    return false;
  }

  out << "GENERATION_START " << generation << endl;

  // save random number generator state
  const vector<int> &state = rng.DumpState();
  if(state.size() != 60) {
    cerr << "ERROR: Incorrect number of state variables for random" << endl;
    cerr << "       number generator during SaveGeneration" << endl;
  }
  vector<int>::const_iterator istate;
  for(istate = state.begin(); istate != state.end(); ++istate) {
    out << *istate << ' ';
  }
  out << endl;

  const vector<CreatureInfo *> &population = ps.Population();

  vector<CreatureInfo *> toSort = population;
  sort(toSort.begin(), toSort.end(), CreatureInfo::Greater);

  vector<CreatureInfo *>::const_iterator ips;
  for(ips=toSort.begin(); ips!=toSort.end(); ++ips) {
    if((*ips) == NULL) { return false; }

    SaveCreature(out, **ips);
  }

  out << "GENERATION_END" << endl;

  return true;
}

bool FileHandler::Output(int fileId, int generation, iPopulationStorage &ps){
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::out)) {
    return false;
  }

  if(i->second->d_file.rdstate()) {
    return false;
  }

  fstream &out = i->second->d_file;
  if(!out.is_open()) {
    return false;
  }

  out << "GENERATION_START " << generation << endl;

  vector<CreatureInfo *> &population = ps.Population();
  vector<CreatureInfo *>::iterator j;
  for(j = population.begin(); j != population.end(); j++){
    out << (*j)->Id() << ","
        << (*j)->ParentId() << ","
        << (*j)->Fitness() << ",";
    const vector<double> fitnesses = (*j)->Fitnesses();
    for(unsigned h = 0; h < fitnesses.size(); h++){
      out << fitnesses.at(h) << ",";
    }
    out << endl;
  }

  out << "GENERATION_END" << endl;
  return true;
  
}

void FileHandler::SaveCreature(ostream &out, const CreatureInfo &c) {
  out << "CREATURE_START" << endl;
  // 1 is the number of replicates, which I don't use anymore, except for
  // loading
  // 1 is the language type, which I no longer use
  out << "CREATURE_DATA " << "1" << " " << 1 << " ";
  out << c.Id() << " " << c.ParentId() << " " << c.Fitness() << endl;
  c.GetCreature().GetGenome().Stringify(out);
  out << "CREATURE_END" << endl;

  return;
}

/*
 * END Save the state for a given generation
 */

/*
 * END Save experiment data
 */

bool FileHandler::OutputCreature(int fileId, const CreatureInfo &ci) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::out)) {
    return false;
  }

  if(i->second->d_file.rdstate()) {
    return false;
  }

  fstream &out = i->second->d_file;
  if(!out.is_open()) {
    return false;
  }

  SaveCreature(out, ci);

  return true;
}

/*
 * BEGIN Load experiment data
 */

/*
 * BEGIN Load experiment Parameters
 */

istream &FileHandler::InputStream(int fileId, bool &success) {
  map<int, FileHandle*>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::in)) {
    success = false;
    return d_defaultStream;
  }

  if(i->second->d_file.rdstate()) {
    success = false;
    return d_defaultStream;
  }

  if(!i->second->d_file.is_open()) {
    success = false;
    return d_defaultStream;
  }

  success = true;
  return i->second->d_file;;
}

bool FileHandler::Input(int fileId, int &populationSize,
			int &generationQuantity, double &timestep) {
  int ivalue;
  double dvalue;

  map<int, FileHandle *>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::in)) {
    return false;
  }

  fstream &in = i->second->d_file;
  if(!in.is_open()) {
    return false;
  }

  streampos curLoc = in.tellg();
  in.seekg(0, ios::beg);

  // you can override the loaded values by specifing them on the commandline
  in >> ivalue;
  if(populationSize <= 0 && ivalue > 0) {
    populationSize = ivalue;
  }

  in >> ivalue;
  if(generationQuantity <= 0 && ivalue > 0) {
    generationQuantity = ivalue;
  }

  in >> dvalue;
  if(timestep <= 0 && dvalue > 0) {
    timestep = dvalue;
  }

  // Dummy input, no longer use language type
  in >> ivalue;

  in.seekg(curLoc, ios::beg);

  return true;
}

/*
 * END Load experiment Parameters
 */

/*
 * BEGIN Load creatures from last generation into the initial population of
 *       the new experiment
 */

bool FileHandler::Input(int fileId, RandomNumberGenerator &rng,
			EnvironmentData &ed, iPopulationStorage &ps,
			LanguageEngine &le) {
  char line[1025];
  int size = 1024;

  map<int, FileHandle *>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::in)) {
    return false;
  }

  fstream &in = i->second->d_file;
  if(!in.is_open()) {
    return false;
  }

  if(!FindLastGeneration(in, line, size)) {
    return false;
  } else if(in.eof()) {
    return true;
  } else if(in.rdstate()) {
    return false;
  }

  if(!LoadRandomNumberGeneratorState(in, rng)) {
    cerr << "ERROR: Either malformed random number states or the random ";
    cerr << "       number generator failed to load the new state." << endl;
    return false;
  }

  vector<int64> ids;
  if(!ps.Ids(ids)) {
    cerr << "ERROR: Failed to get ids of the population." << endl;
    return false;
  }

  vector<int64>::const_iterator iids = ids.begin();

  // Load up to a specified number of creatures from the last generation data,
  // and can have a random number state and no creatures
  while(!in.eof()) {
    in.getline(line, size, '\n');

    // Convert the character stream to a stringstream for ease of parsing
    stringstream linestream;
    linestream << line;

    string testLine;
    linestream >> testLine;

    if(testLine == "CREATURE_START") {
      int replicates;
      int language;
      int64 id, parentId;
      double fitness;
      string genomeString;
      if(!LoadCreature((istream &)in, line, size, replicates, language,
		       id, parentId, fitness, genomeString)) {
	return false;
      }

      for(int i=0; i<replicates; i++) {
	if(iids == ids.end()) { return true; }

	if(!ps.Replace(*iids, id, genomeString, ed, le)) {
	  return false;
	}

	iids++;
      }
    } else if(testLine == "GENERATION_END") {
      break;
    }
  }

  return true;
}

bool FileHandler::FindLastGeneration(istream &in, char *line, int size) {
  bool found = false;
  int ivalue;
  streampos currentGeneration = in.tellg();

  // Find the last generation and record its location in the file
  while(!in.eof()) {
    in.getline(line, size, '\n');

    // Convert the character stream to a stringstream for ease of parsing
    stringstream linestream;
    linestream << line;

    string testLine;
    linestream >> testLine;

    if(testLine == "GENERATION_START") {
      found = true;
      currentGeneration = in.tellg();

      // generation number 
      linestream >> ws;
      char cvalue = linestream.peek();
      if(cvalue < '0' || cvalue > '9') {
	cerr << "ERROR: Malformed input, expecting integer for the ";
	cerr << "       generation number." << endl;
	return false;
      }
      linestream >> ivalue;
    }
  }
  // If the file contains no generation information then it is considered
  // empty and the population is initialized with default creatures.
  // Will return true and at the end of the file.
  if(!found) {
    return true;
  }

  // Once the beginning of the last generation has been found, move to it
  in.clear();
  streampos end = in.tellg();
  in.seekg(currentGeneration - end, ios::cur);

  return true;
}

bool FileHandler::LoadRandomNumberGeneratorState(istream &in,
						 RandomNumberGenerator &rng) {
  int ivalue;
  vector<int> state;
  state.reserve(60);
  while(state.size() < 60 && !in.eof()) {
    in >> ivalue;
    state.push_back(ivalue);
  }

  if(state.size() != 60 || !rng.LoadState(state)) {
    return false;
  }

  return true;
}

bool FileHandler::LoadCreature(std::istream &in, char *line, int size,
			       int &replicates,
			       int &language, int64 &id, int64 &parentId,
			       double &fitness, std::string &genomeString) {
  while(!in.eof()) {
    // not worrying if the line exceeds size
    in.getline(line, size, '\n');
    string testLine(line);

    stringstream tempStream;
    tempStream << line;
    string tempString;
    tempStream >> tempString;

    if(tempString == "CREATURE_END") {
      return true;
    } else if(tempString == "CREATURE_DATA") {
      tempStream >> ws;
      tempStream >> replicates;
      tempStream >> ws;
      tempStream >> language;
      tempStream >> ws;
      tempStream >> id;
      tempStream >> ws;
      tempStream >> parentId;
      tempStream >> ws;
      tempStream >> fitness;
    } else {
      genomeString.append(testLine+'\n');
    }
  }

  return false;
}

/*
 * END Load creatures from last generation into the initial population of
 *     the new experiment
 */

/*
 * END Load experiment data
 */

bool FileHandler::InputCreature(int fileId, CreatureInfo &ci,
				string &genomeString) {
  char line[1025];
  int size = 1024;

  int64 id, parentId;

  map<int, FileHandle *>::iterator i = d_files.find(fileId);
  if(i == d_files.end() || i->second == NULL ||
     !(i->second->d_type & ios::in)) {
    return false;
  }

  fstream &in = i->second->d_file;
  if(!in.is_open()) {
    return false;
  }

  while(!in.eof()) {
    in.getline(line, size, '\n');

    // Convert the character stream to a stringstream for ease of parsing
    stringstream linestream;
    linestream << line;

    string testLine;
    linestream >> testLine;

    if(testLine == "CREATURE_START") {
      int disregardReplicates;
      int disregardLanguage;
      if(!LoadCreature((istream &)in, line, size, disregardReplicates,
		       disregardLanguage, id, parentId, ci.Fitness(),
		       genomeString)) {
	return false;
      } else {
	break;
      }
    }
  }

  return true;
}
