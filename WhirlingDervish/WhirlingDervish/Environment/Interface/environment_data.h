#ifndef __ENVIRONMENT_DATA_H__
#define __ENVIRONMENT_DATA_H__

#include <string>

#include "random.h"
#include "language_engine.h"
#include "shape_engine.h"
#include "population_storage.h"
//#include "hfc_population_storage.h"
//#include "hfc_population_storage_4.h"
#include "hfc_population_storage_6.h"
#include "run_settings.h"
#include "physics_settings.h"
#include "ec_settings.h"

class FileHandler;

class EnvironmentData {
public:
  EnvironmentData(void);
  virtual ~EnvironmentData(void);

  // LanguageEngine won't be initialized without this function
  bool InitializeData(void);
  bool LoadConfigurationFiles(FileHandler &,
			      const std::string &runSettingsFilename,
			      const std::string &physicsSettingsFilename,
			      const std::string &ecSettingsFilename,
			      const std::string &instructionSetFilename);

  RandomNumberGenerator &GetRng(void) { return d_random; }
  ShapeEngine &GetShapeEngine(void) { return d_shapeEngine; }
  LanguageEngine &GetLanguageEngine(void) { return d_languageEngine; }
  iPopulationStorage &GetPopulationStorage(void) {
    return (iPopulationStorage &)d_populationStorage;
  }

  RunSettings &GetRunSettings(void) { return d_runSettings; }
  PhysicsSettings &GetPhysicsSettings(void) { return d_physicsSettings; }
  ECSettings &GetECSettings(void) { return d_languageEngine.GetECSettings(); }

  double CurrentTime(void) const { return d_currentTime; }
  unsigned int Generation(void) const { return d_generation; }

  const unsigned int  Cycle(void)  const{return d_cycle;}
        unsigned int &Cycle(void) {return d_cycle;}

  const double  AvgPoints(void)        const { return d_avgPoints; }
        double &AvgPoints(void)              { return d_avgPoints; }
  const unsigned int  MaxPoints(void)  const { return d_maxPoints; }
        unsigned int &MaxPoints(void)        { return d_maxPoints; }
  const double  AvgSprings(void)       const { return d_avgSprings; }
        double &AvgSprings(void)             { return d_avgSprings; }
  const unsigned int  MaxSprings(void) const { return d_maxSprings; }
        unsigned int &MaxSprings(void)       { return d_maxSprings; }

  const double  AvgFitness(void) const { return d_avgFitness; }
        double &AvgFitness(void)       { return d_avgFitness; }
  const double  MaxFitness(void) const { return d_maxFitness; }
        double &MaxFitness(void)       { return d_maxFitness; }
  const double  AvgHeight(void)  const { return d_avgHeight; }
        double &AvgHeight(void)        { return d_avgHeight; }
  const double  MaxHeight(void)  const { return d_maxHeight; }
        double &MaxHeight(void)        { return d_maxHeight; }
  const double  AvgMove(void)    const { return d_avgMove; }
        double &AvgMove(void)          { return d_avgMove; }
  const double  MaxMove(void)    const { return d_maxMove; }
        double &MaxMove(void)          { return d_maxMove; }

  const double  BestFitness(void)       const { return d_bestFitness; }
	double &BestFitness(void)             { return d_bestFitness; }
  const unsigned int  BestPoints(void)  const { return d_bestPoints; }
	unsigned int &BestPoints(void)        { return d_bestPoints; }
  const unsigned int  BestSprings(void) const { return d_bestSprings; }
	unsigned int &BestSprings(void)       { return d_bestSprings; }
  const double  BestHeight(void)        const { return d_bestHeight; }
	double &BestHeight(void)              { return d_bestHeight; }
  const double  BestMove(void)          const { return d_bestMove; }
	double &BestMove(void)                { return d_bestMove; }
  const double  BestCOM(void)          const { return d_bestCOM; }
	double &BestCOM(void)                { return d_bestCOM; }
  const bool  BestMoved(void)          const { return d_bestMoved; }
	bool &BestMoved(void)                { return d_bestMoved; }

  int OutputFileId(void) const { return d_outputFileId; }

protected:
  RandomNumberGenerator d_random;
  RandomNumberGenerator d_randomTemp;
  ShapeEngine d_shapeEngine;
  LanguageEngine d_languageEngine;

  PopulationStorage d_populationStorage;
  //hfcPopulationStorage4 d_populationStorage;  //AHFC
  //hfcPopulationStorage5 d_populationStorage;  //clades
  //hfcPopulationStorage6 d_populationStorage;    //CHFC

  RunSettings d_runSettings;
  PhysicsSettings d_physicsSettings;

  double d_currentTime;
  unsigned int d_generation;
  unsigned int d_cycle;
  int d_outputFileId;
  int d_outputCreatureFitId;

  double d_avgPoints;
  unsigned int d_maxPoints;
  double d_avgSprings;
  unsigned int d_maxSprings;

  double d_avgFitness;
  double d_maxFitness;
  double d_avgHeight;
  double d_maxHeight;
  double d_avgMove;
  double d_maxMove;

  double d_bestFitness;
  unsigned int d_bestPoints;
  unsigned int d_bestSprings;
  double d_bestHeight;
  double d_bestMove;
  double d_bestCOM;
  bool d_bestMoved;

  bool d_dataInitialized;

  bool LoadConfigurationFile(FileHandler &, Setting &,
			     const std::string &filename);
};

#endif
