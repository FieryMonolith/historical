#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <fstream>
#include <string>
#include <map>

class RandomNumberGenerator;
class EnvironmentData;
class iPopulationStorage;
class LanguageEngine;
class CreatureInfo;

class FileHandler {
private:
  class FileHandle {
  public:
    FileHandle(void);
    ~FileHandle(void);

    int d_id;
    int d_type;
    std::fstream d_file;
    std::string d_filename;
  };

public:
  FileHandler(void);
  ~FileHandler(void);

  int Create(const std::string &filename,
	     std::fstream::openmode mode=std::ios::in|std::ios::out);
  void Close(int fileId);

  std::ostream &OutputStream(int fileId, bool &success);
  bool Output(int fileId, int populationSize, int generationQuantity,
	      double timestep);
  bool Output(int fileId, int generation, RandomNumberGenerator &,
	      iPopulationStorage &);
  bool Output(int fileID, int generation, iPopulationStorage &);

  bool OutputCreature(int fileId, const CreatureInfo &);
  bool OutputCreatureFitness(int fileID, const CreatureInfo&);



  std::istream &InputStream(int fileId, bool &success);
  bool Input(int fileId, int &populationSize, int &generationQuantity, 
	     double &timestep);
  bool Input(int fileId, RandomNumberGenerator &, EnvironmentData &,
	     iPopulationStorage &, LanguageEngine &);
  bool InputCreature(int fileId, CreatureInfo &ci, std::string &genomeString);

private:
  std::fstream d_defaultStream;
  std::map<int, FileHandle *> d_files;
  int d_currentId;

  void SaveCreature(std::ostream &, const CreatureInfo &);

  bool FindLastGeneration(std::istream &, char *, int);
  bool LoadRandomNumberGeneratorState(std::istream &, RandomNumberGenerator &);
  bool LoadCreature(std::istream &, char *, int, int &replicates,
		    int &language, int64 &id, int64 &parentId,
		    double &fitness, std::string &genomeString);
};

#endif
